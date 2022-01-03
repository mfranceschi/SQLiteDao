from dataclasses import dataclass

from .my_types import (Case_t, GeneralConfigs, GeneratedFileToWrite,
                       TableFromYaml, YamlFile)

HEADER_TO_INCLUDE = "TheHeader.hpp"
NAMESPACES: list[str] = ["custom_ns"]
PARENT_TABLE_CLASS = "AbstractTable"


@dataclass
class StatementData:
    sql_statement: str
    constant_string_name: str
    method_name: str


class _Helpers:
    @staticmethod
    def _make_table_class_name(table_name: str, case: Case_t) -> str:
        if case == "snake_case":
            return f"{table_name}_table"
        else:
            return f"{table_name}Table"

    @staticmethod
    def _drop_table_if_exists(table_name: str) -> str:
        return f"DROP TABLE IF EXISTS {table_name}; "

    @staticmethod
    def _create_table(table_data: TableFromYaml) -> str:
        column_definitions = (
            f"{column_data.name} {column_data.type.upper()}"
            for column_data
            in table_data.columns
        )
        table_constraints = []
        if table_data.primary_key:
            table_constraints.append(f"PRIMARY KEY ({table_data.primary_key})")
        return f"CREATE TABLE {table_data.name} ({', '.join((*column_definitions, *table_constraints))}); "

    @staticmethod
    def _drop_table(table_name: str) -> str:
        return f"DROP TABLE {table_name}; "


class CppCodeGenerator:

    def __init__(self, yaml_data: YamlFile, file_name: str) -> None:
        self.yaml_data = yaml_data
        self.includes: list[str] = [f'#include "{HEADER_TO_INCLUDE}"']
        self.file_name = file_name

    def generate(self) -> list[GeneratedFileToWrite]:
        # Generate definitions of classes for tables
        table_class_definitions = []
        for table_from_yaml in self.yaml_data.tables:
            table_class_def_text = self._make_table_class_definition(
                table_from_yaml,
                self.yaml_data.generalConfigs
            )
            table_class_definitions.append(table_class_def_text)
        table_class_definitions_text = "\n\n".join(self._make_table_class_definition(
            table_from_yaml,
            self.yaml_data.generalConfigs
        ) for table_from_yaml in self.yaml_data.tables)

        # Gathering the includes later in case we have to add additional includes beforehands.
        includes_text = "\n".join(self.includes)

        texts = (
            includes_text,
            "".join(f"namespace {ns} {{" for ns in NAMESPACES),
            table_class_definitions_text,
            "}\n" * len(NAMESPACES)
        )

        generated_file_name = "{}.hpp".format(
            self.yaml_data.generalConfigs.name if self.yaml_data.generalConfigs.name else self.file_name
        )
        generated_file = GeneratedFileToWrite(
            file_name=generated_file_name,
            content="\n\n".join(texts)
        )
        return [generated_file]

    def _make_table_class_definition(self, table_from_yaml: TableFromYaml, configs: GeneralConfigs):

        class_name = _Helpers._make_table_class_name(
            table_from_yaml.name,
            configs.case
        )

        text = ""

        text += f"class {class_name} : public {PARENT_TABLE_CLASS} {{\n"

        text += f"public: \n"
        text += f"  explicit {class_name}(SQLite::Database& db) : {PARENT_TABLE_CLASS}(db) {{}} \n\n"

        text += "protected: \n"

        statement_texts = []

        for statement in (
            StatementData(
                sql_statement=_Helpers._create_table(table_from_yaml),
                constant_string_name="CREATE_TABLE_STATEMENT",
                method_name="getCreateTableStatement"),
            StatementData(
                sql_statement=_Helpers._drop_table_if_exists(
                    table_from_yaml.name),
                constant_string_name="DROP_TABLE_IF_EXISTS_STATEMENT",
                method_name="getDropTableIfExistsStatement"),
            StatementData(
                sql_statement=_Helpers._drop_table(table_from_yaml.name),
                constant_string_name="DELETE_TABLE_STATEMENT",
                method_name="getDeleteTableStatement"),
        ):
            statement_texts.append(
                f"  static constexpr const char* {statement.constant_string_name} = \"{statement.sql_statement}\";\n" +
                f"  const char* {statement.method_name}() const override {{ return {statement.constant_string_name}; }}\n"
            )

        text += "\n".join(statement_texts)
        text += "};"

        return text
