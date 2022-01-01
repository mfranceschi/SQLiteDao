# SQLiteDao

An attempt to generate DAO code for a local SQL database for use in C++17

## Expected usage

**Work in progress here!**

1. Create and fill a YAML file that describes your DB tables and other metadata.
2. CMake - Set a variable `DAO_DESCRIPTOR` that contains the absolute path to that YAML.
3. CMake - Add this subdirectory and link your target(s) to library `Generated_Your_Dao_By_Mf`.
4. C++ - Include file `<YaHooDao.hpp>` in your code and use the table classes!

## Possible future improvements

- Set multiple YAML files. They will still result in a single CMake target.
