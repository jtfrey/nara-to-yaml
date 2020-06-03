# nara-to-yaml

A program to convert compacted NARA data archives to YAML.  The archived data includes character data in EBCDIC encoding; the text is reencoded to ASCII (in lossy fashion for out-of-scope EBCDIC code points).

The program accepts one or more file paths as arguments.  Each file is read record-by-record and records are output to stdout in YAML format.  Any problems that occur should produce output on stderr.

```
$ ./nara-to-yaml ~/RG441.ESS.CVRGY70
- recordType: district
  schoolSystemCode: 1000030
  oeCode1970: 1000030
  srgCode: ""
  systemName: "ALEXANDER CY"
  systemStreetAddr: ""
  systemCity: "ALEXANDER"
  systemCounty: "TALLAPOOSA"
  systemState: "ALA"
  systemZipCode: "35010"
    :
```

## On-disk structure

The file format uses EBCDIC character strings of fixed length (right-padded with whitespace) and big-endian binary fields.  At the first level, the file is organized as variable-length chunks containing all records for a state:

```
+----------------+----------------+-------------- ~ --+
| 16-bit, b.e.   | 16-bit         | State chunk       |
| = 4 + byte     | = 0 (unused)   |                   |
|   length of    |                |                   |
|   chunk        |                |                   |
+----------------+----------------+-------------- ~ --+
```

The state chunks occur in alphabetical order in the file.

The state chunk is organized in similar fashion, with each record (of type district, school, or classroom) preceded by its size:

```
+----------------+----------------+-------------- ~ --+
| 16-bit, b.e.   | 16-bit         | Record data       |
| = 4 + byte     | = 0 (unused)   |                   |
|   length of    |                |                   |
|   record       |                |                   |
+----------------+----------------+-------------- ~ --+
```

The structure of the record data varies by record type (see the `nara_district.h`, `nara_school.h`, and `nara_classroom.h` headers).  The first 32-bits of the record data is always the type id, though:

```
+--------------------------------+-------------------- ~ --+
| 32-bit, b.e.                   | Additional fields       |
| record type                    | vary by record          |
| 1 = district, 2 = school,      | type                    |
| 3 = classroom                  |                         |
+--------------------------------+-------------------- ~ --+
```

## Structure of the code

The on-disk layout of the components of the NARA data archive are presented in header files:

- `nara_state_header.h` : the 4-bytes defining the size of the first-level record containing all records for a single state
- `nara_record_header.h` : the 4-bytes defining the size of a district/school/classroom record
- `nara_record.h` : the field(s) common to each record type (district/school/classroom) and a generic interface to the read, output to YAML, and destroy in-memory representations of records

The on-disk layout of each of the three record types themselves and key enumerations used to simplify the structures are to be found in the individual public headers:

- `nara_district.h`
- `nara_school.h`
- `nara_classroom.h`

As currently written, only the internal per-type implementations access the fields of the record; but future changes (e.g. filtering records in the main() function) may need access to the structures and could use the appropriate header file and a type cast to do so.

Please note that as written the program is **not** thread-safe.  Since reading the file is a serial operation, there's little reason to parallelize the program and require thread safety anyway, though.

## Building the program

CMake 3.11 or newer is required to build this program.  Redhat Linux tends to have much older versions; on our **caviness** cluster newer versions are provided by VALET:

```
$ vpkg_require cmake
```

In the source directory, start by creating a build directory:

```
$ mkdir build
$ cd build
```

Configure the build:

```
$ cmake -DCMAKE_BUILD_TYPE=Release ..
```

Build the program:

```
$ make
```

You should now have a `nara-to-yaml` executable in that `build` directory.  It can be installed by setting `CMAKE_INSTALL_PREFIX` when you configure the build directory with `cmake` and doing:

```
$ make install
```

