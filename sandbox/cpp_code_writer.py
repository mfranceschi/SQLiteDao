
from typing import List, Optional
from my_types import YamlFile
from my_types import ColumnType_t, TableFromYaml

from my_types import Case_t


HEADER_TO_INCLUDE = "TheHeader.hpp"
NAMESPACES: List[str] = []
PARENT_TABLE_CLASS = "AbstractTable"


class CppCodeGenerator:

    def __init__(self, yaml_data: YamlFile) -> None:
        self.yaml_data = yaml_data
        self.includes: List[str] = [f'#include "{HEADER_TO_INCLUDE}"']

    def generate(self) -> str:
        case_config = self.yaml_data.generalConfigs.case

        table_class_definitions = []
        for table_from_yaml in self.yaml_data.tables:
            text = ""

            class_name = self._make_table_class_name(
                table_from_yaml.name, case_config)

            text += f"class {class_name} : public {PARENT_TABLE_CLASS} {{\n"

            # TODO content of class
            text += "protected: \n"

            for statement in (
                (
                    "CREATE_TABLE_STATEMENT",
                    self._create_table(table_from_yaml),
                    "getCreateTableStatement"
                ),
                (
                    "DROP_TABLE_IF_EXISTS_STATEMENT",
                    self._drop_table_if_exists(table_from_yaml.name),
                    "getDropTableIfExistsStatement"
                ),
                (
                    "DELETE_TABLE_STATEMENT",
                    self._drop_table_if_exists(table_from_yaml.name),
                    "getDeleteTableStatement"
                ),
            ):
                string_name = statement[0]
                string_value = statement[1]
                method_name = statement[2]

                text += f"static constexpr const char* {string_name} = \"{string_value}\"; \n"
                text += f"const char* {method_name}() const override {{ return {string_name}; }}\n\n"

            text += "};"

            table_class_definitions.append(text)

        table_class_definitions_text = "\n\n".join(table_class_definitions)

        # Gathering the includes later in case we had to add additional includes.
        includes_text = "\n".join(self.includes)

        texts = [
            includes_text,
            "".join(f"namespace {ns} {{ \n" for ns in NAMESPACES),
            table_class_definitions_text,
            "\n" * len(NAMESPACES)
        ]

        return "\n\n".join(texts)

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
            table_constraints.append(f"PRIMARY KEY {table_data.primary_key}")
        return f"CREATE TABLE {table_data.name} ({', '.join((*column_definitions, *table_constraints))}); "

    @staticmethod
    def _drop_table(table_name: str) -> str:
        return f"DROP TABLE {table_name}; "
