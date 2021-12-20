
from dataclasses import dataclass
from typing import List, Literal, Union

Case_t = Union[
    Literal["PascalCase"],
    Literal["camelCase"],
    Literal["camelBack"],
    Literal["snake_case"]
]

ColumnType_t = Union[Literal["integer"], Literal["text"]]


@dataclass
class GeneralConfigs:
    case: Case_t = "PascalCase"


@dataclass
class ColumnFromYaml:
    name: str
    type: ColumnType_t


@dataclass
class TableFromYaml:
    name: str
    columns: List[ColumnFromYaml]
    primary_key: str


@dataclass
class YamlFile:
    generalConfigs: GeneralConfigs
    tables: List[TableFromYaml]
