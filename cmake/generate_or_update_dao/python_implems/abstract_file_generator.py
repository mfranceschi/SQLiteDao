from abc import abstractmethod

from .my_types import GeneratedFileToWrite


class Abstract_File_Generator:
    @abstractmethod
    def generate(self) -> list[GeneratedFileToWrite]:
        pass
