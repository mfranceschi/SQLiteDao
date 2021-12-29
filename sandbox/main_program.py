import argparse
from pathlib import Path
import sys

from cpp_code_generator import CppCodeGenerator
from cmake_file_generator import CMake_File_Generator
from yaml_parser import parse

assert sys.version_info >= (3, 9), "You need at least Python 3.9"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description='Generate CPP code from YAML file for SQLite DAO.')
    parser.add_argument(
        '--yaml_file',
        metavar="yaml_file",
        type=str,
        required=False,  # TODO true
        default=str(Path(__file__).parent / "simple-yaml.yaml"),
        help='Source YAML file (read-only)')
    parser.add_argument(
        '--path_to_write',
        metavar="path_to_write",
        type=str,
        required=False,  # TODO true
        default=str(Path(__file__).parent / "new_cpp_lib" / "Xyz"),
        help='We will write the CPP files in this folder.')

    return parser.parse_args()


def main():
    arguments = parse_args()
    yaml_file: Path = Path(arguments.yaml_file).absolute()
    assert yaml_file.exists(), "Couldn't find the YAML file. Please prefer absolute paths."

    cpp_file_to_write: str = arguments.path_to_write

    yaml_content = parse(yaml_file)

    cpp_code = CppCodeGenerator(yaml_content).generate()
    print(*[code.content for code in cpp_code])

    cmake_code = CMake_File_Generator(
        files_to_compile=[str(cpp_file_to_write) + cpp_file.suffix for cpp_file in cpp_code]).generate()
    print(cmake_code[0].content)

    for file in cpp_code:
        file_name_to_write = str(cpp_file_to_write) + file.suffix
        with open(file_name_to_write, "w") as f:
            f.write(file.content)


if __name__ == "__main__":
    main()
