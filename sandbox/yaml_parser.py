from typing import List

from my_types import ColumnFromYaml, GeneralConfigs, TableFromYaml, YamlFile


def parse(yamlAsDict) -> YamlFile:
    general_config = GeneralConfigs(case=yamlAsDict["case"])

    tables: List[TableFromYaml] = []

    for table in yamlAsDict["tables"]:
        columns = [ColumnFromYaml(**column) for column in table["fields"]]

        tables.append(TableFromYaml(
            name=table["name"], primary_key=table["primary_key"], columns=columns))

    return YamlFile(generalConfigs=general_config, tables=tables)
