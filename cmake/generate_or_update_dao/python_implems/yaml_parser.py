from os import PathLike
from typing import List

import yaml

from .my_types import (AnyTypeWithName, ColumnFromYaml, GeneralConfigs,
                       TableFromYaml, YamlFile)


def check_names_are_unique_and_correct_chars(list_of_objects_with_names: list[AnyTypeWithName], error_prefixes: str):
    names: set[str] = set()
    for object_with_name in list_of_objects_with_names:
        name = object_with_name.name
        assert name.isidentifier(), \
            f"{error_prefixes} - Name {name} contains invalid characters."
        names.add(name.lower())

    assert len(names) == len(list_of_objects_with_names), \
        f"{error_prefixes} - There are name duplicate "


def parse(yaml_file_name: PathLike) -> YamlFile:
    with open(yaml_file_name, "r") as f:
        yamlAsDict: dict[dict] = yaml.load(f, Loader=yaml.Loader)

    general_config = GeneralConfigs(
        case=yamlAsDict["case"],
        name=yamlAsDict.get("name", None)
    )

    tables: List[TableFromYaml] = []

    for table in yamlAsDict["tables"]:
        table: dict
        table_name = table["name"]
        columns = [ColumnFromYaml(**column) for column in table["fields"]]
        check_names_are_unique_and_correct_chars(
            columns,
            f"Columns of table {table_name}"
        )

        tables.append(
            TableFromYaml(
                name=table_name,
                primary_key=table["primary_key"],
                columns=columns,
                predefined_read_queries=table.get(
                    "read_queries", {}
                ).get("predefined", [])
            )
        )

    check_names_are_unique_and_correct_chars(tables, f"List of tables")

    return YamlFile(generalConfigs=general_config, tables=tables)
