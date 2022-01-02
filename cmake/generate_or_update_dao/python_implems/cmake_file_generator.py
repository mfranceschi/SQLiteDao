
from os import PathLike
from pathlib import Path

from .abstract_file_generator import Abstract_File_Generator
from .my_types import GeneratedFileToWrite

STATIC_LIBRARY_NAME = "GeneratedSQLiteCodeClassesByMyMagicDAO"
STATIC_LIB_TO_LINK_TO = "MyMartinBaseWrapper"


# Raw content of the CMakeLists file. Use str.format(**kwargs) to fill it appropriately.
CMAKE_CODE = """add_library(
    {static_library_name} STATIC 
    {file_paths}
)
target_link_libraries(
    {static_library_name} 
    PUBLIC 
        {static_libs_to_link_to}
    )
"""


class CMake_File_Generator(Abstract_File_Generator):
    def __init__(self, files_to_compile: list[PathLike]) -> None:
        self.files_to_compile = [Path(file).name for file in files_to_compile]

    def generate(self) -> list[GeneratedFileToWrite]:
        file_content = self._generate_impl(
            static_library_name=STATIC_LIBRARY_NAME,
            static_libs_to_link_to=[STATIC_LIB_TO_LINK_TO],
            file_paths=self.files_to_compile
        )
        generated_file = GeneratedFileToWrite(
            file_name="CMakeLists.txt",
            content=file_content
        )
        return [generated_file]

    @staticmethod
    def _generate_impl(static_library_name: str, static_libs_to_link_to: list[str], file_paths: list[str]) -> str:
        return CMAKE_CODE.format(
            static_library_name=static_library_name,
            file_paths=" ".join(file_paths),
            static_libs_to_link_to=" ".join(static_libs_to_link_to)
        )
