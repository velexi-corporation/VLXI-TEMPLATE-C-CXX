C/C++ Package Template (0.1.1)
==============================

___Authors___  
Kevin T. Chu `<kevin@velexi.com>`

------------------------------------------------------------------------------

Contents
--------

1. [Overview][#1]

    1.1. [Software Dependencies][#1.1]

    1.2. [Directory Structure][#1.2]

    1.3. [License][#1.3]

2. [Usage][#2]

    2.1. [Setting Up][#2.1]

------------------------------------------------------------------------------

## 1. Overview

This package template is intended to streamline the process of setting up a
C/C++ software package.

### 1.1. Software Dependencies

#### Base Requirements

* CMake (>=3.10)
* C compiler (compatible with C11 standard)
* C++ compiler (compatible with C++11 standard)

### 1.2. Directory Structure

    README.md
    AUTHORS.template
    LICENSE.template
    LICENSE-THIRD-PARTY.template
    README.md.template
    RELEASE-NOTES.md.template
    CMakeLists.txt
    cmake/
    cmake_uninstall.cmake.in
    docs/
    ext/
    include/
    spikes/
    src/
    tests/
    template-docs/
    template-docs/extras/

* `README.md`: this file (same as `README-C-C++-Template.md` in the
  `template-docs` directory)

* `*.template`: template files for the package

    * Template files are indicated by the `template` suffix and contain
      template parameters denoted by double braces (e.g. `{{ PKG_NAME }}`).
      Template files are intended to simplify the set up of the package. When
      used, they should be renamed to remove the `template` suffix.

* `CMakeLists.txt`: top-level CMake configuration file that contains a set of
  directives for building, testing, installing, and packaging C/C++ software
  libraries

    * `cmake`: directory containing CMake macros used by the CMake build system
      generator

    * `cmake_uninstall.cmake.in`: template used by CMake to generate the
      package uninstallation script

* `docs `: directory where package documentation should be placed

* `ext`: directory containing external software shipped with the package.
  The license for any external software shipped with the package should be
  included in the `LICENSE-THIRD-PARTY` file.

    * `ext/googletest` is included with the template so that tests can be
      be built and run without requiring an internet connection (to download
      `googletest` from a GitHub).

* `include `: directory containing package-wide header files (and template
  header files)

* `spikes `: directory where experimental code should be placed

* `src`: directory for package source code

    * This template ships with the example `CMakeLists.txt` files and
      example C++ header and implementation files.

* `tests`: directory for test code

    * This template ships with the example `CMakeLists.txt` files and
      example C++ header and implementation files.

* `template-docs`: directory containing documentation for this package template

    * `template-docs/extras`: directory containing example and template files

### 1.3. License

The contents of this package template are covered under the
`LICENSE-C-C++-Template` file contained in the `template-docs` of this package.

------------------------------------------------------------------------------

## 2. Usage

### 2.1. Setting Up

1. Update the names and content of all files in the `src` and `tests`
   directories to be consistent with the choice of package name.

    * Replace all references to the `xyz` in filenames and source code.

2. Rename all of the template files with the `template` suffix removed
   (overwrite the original `README.md` file) and replace all template
   parameters with package-appropriate values.

------------------------------------------------------------------------------

[-----------------------------INTERNAL LINKS-----------------------------]: #

[#1]: #1-overview
[#1.1]: #11-software-dependencies
[#1.2]: #12-directory-structure
[#1.3]: #13-license

[#2]: #2-usage
[#2.1]: #21-setting-up
