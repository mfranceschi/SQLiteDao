from os import PathLike
from typing import List

import yaml

from my_types import ColumnFromYaml, GeneralConfigs, TableFromYaml, YamlFile


def parse(yaml_file_name: PathLike) -> YamlFile:
    with open(yaml_file_name, "r") as f:
        yamlAsDict = yaml.load(f, Loader=yaml.Loader)
    general_config = GeneralConfigs(case=yamlAsDict["case"])

    tables: List[TableFromYaml] = []

    for table in yamlAsDict["tables"]:
        columns = [ColumnFromYaml(**column) for column in table["fields"]]

        tables.append(
            TableFromYaml(
                name=table["name"],
                primary_key=table["primary_key"],
                columns=columns
            )
        )

    return YamlFile(generalConfigs=general_config, tables=tables)
