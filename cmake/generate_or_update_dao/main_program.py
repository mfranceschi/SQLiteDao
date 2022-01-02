import argparse
import logging
import sys
from pathlib import Path

from python_implems.cpp_code_generator import CppCodeGenerator
from python_implems.yaml_parser import parse

assert sys.version_info >= (3, 9), "You need at least Python 3.9"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description='Generate CPP code from YAML file for SQLite DAO.')
    parser.add_argument(
        '--yaml_file',
        type=str,
        required=False,  # TODO true
        default=str(Path(__file__).parent / "simple-yaml.yaml"),
        help='Source YAML file (read-only)')
    parser.add_argument(
        '--output_folder',
        type=str,
        required=False,  # TODO true
        default=str(Path(__file__).parent / "new_cpp_lib"),
        help='We will write the CPP file(s) in this folder.')

    return parser.parse_args()


def main():
    arguments = parse_args()

    # Check YAML file exists
    yaml_file = Path(arguments.yaml_file)
    assert yaml_file.is_absolute(), "Please use absolute paths (for YAML)."
    assert yaml_file.is_file(), "Couldn't find the YAML file."
    yaml_file_name_no_ext = yaml_file.with_suffix("").name

    # Check output folder is okay
    output_folder = Path(arguments.output_folder)
    assert output_folder.is_absolute(), "Please use absolute paths (for output folder)."
    if output_folder.exists():
        assert output_folder.is_dir(), "Output folder exists and is not a directory."
    else:
        output_folder.mkdir()

    # Parse the YAML file
    yaml_content = parse(yaml_file)

    # C++ code generation
    generated_cpp_files = CppCodeGenerator(
        yaml_content, file_name=yaml_file_name_no_ext
    ).generate()
    logging.debug("Generated C++ files: %s", generated_cpp_files)

    # Writing files
    for file in generated_cpp_files:
        file_name_to_write = output_folder / file.file_name
        with open(file_name_to_write, "w") as f:
            f.write(file.content)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
