
from dataclasses import dataclass
from typing import Any, Literal, Optional, Union

Case_t = Union[
    Literal["PascalCase"],
    Literal["camelCase"],
    Literal["camelBack"],
    Literal["snake_case"]
]
PredefinedReadQueries_t = Union[
    Literal["listAll"],
    Literal["countAll"]
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
class QueryFromYaml:
    pass


@dataclass
class TableFromYaml:
    name: str
    columns: list[ColumnFromYaml]
    primary_key: str
    predefined_read_queries: list[PredefinedReadQueries_t]


@dataclass
class YamlFile:
    generalConfigs: GeneralConfigs
    tables: list[TableFromYaml]


@dataclass
class GeneratedFileToWrite:
    file_name: str
    content: Union[str, list[str]]

    def __repr__(self) -> str:
        return f"GeneratedFileToWrite(file_name='{self.file_name}', content=str of length {len(self.content)})"


@dataclass
class AnyTypeWithName:
    name: str
