
from dataclasses import dataclass
from typing import Any, List, Literal, Optional, Union

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
    name: Optional[str] = None


@dataclass
class ColumnFromYaml:
    name: str
    type: ColumnType_t
    nullable: Optional[bool] = False
    default_value: Optional[Any] = None


@dataclass
class TableFromYaml:
    name: str
    columns: List[ColumnFromYaml]
    primary_key: str


@dataclass
class YamlFile:
    generalConfigs: GeneralConfigs
    tables: List[TableFromYaml]


@dataclass
class GeneratedFileToWrite:
    file_name: str
    content: Union[str, list[str]]


@dataclass
class AnyTypeWithName:
    name: str
