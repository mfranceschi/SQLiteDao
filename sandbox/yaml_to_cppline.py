
import yaml
import pathlib
from cpp_code_writer import CppCodeGenerator

from yaml_parser import parse

yaml_file = pathlib.Path(__file__).parent / "simple-yaml.yaml"
cpp_file_to_write = pathlib.Path(__file__).parent / "new_cpp_lib" / "Xyz.cpp"


with open(yaml_file, mode="r") as f:
    yamlContent = parse(yaml.load(f, Loader=yaml.Loader))
print(yamlContent)

cpp_code = CppCodeGenerator(yamlContent).generate()
print(cpp_code)

with open(cpp_file_to_write, "w") as f:
    f.write(cpp_code)
