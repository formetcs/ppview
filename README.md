# PlanPro Viewer

PlanPro Viewer is a visualization and evaluation tool for digital railway plans in the [PlanPro](http://www.dbinfrago.com/planpro) format.
Originally designed as a debugging tool for the Automated ETCS Planning Tool [EPlan](https://github.com/formetcs/eplan), the main capabilities are:

- Drawing of a simplified, but complete and full-scale track layout plan
- Inspecting of object attributes
- Monitoring of forward and backward references between objects
- Calculation of the relative distance between two objects
- Extraction of binary attachments from the PlanPro file
- Comparison of different planning states (start and end state)


## Who should use PlanPro Viewer?

PlanPro Viewer provides a technical view of a railway plan, which is not familiar to a railway planner.
Therefore it is not intended to be used as a real planning tool in daily workflows.
Instead it is designated for

- Software developers, who develop PlanPro-compatible tools and want to test the interfaces
- (Experienced) planners, who want to get a deeper insight into a plan or search for errors

Knowledge of the PlanPro specifications is highly recommended to work with PlanPro Viewer.


## Download

We do not provide any executable binaries.
Please build PlanPro Viewer from source using the instructions below.


## Building from Source

See file [BUILD.md](BUILD.md) for build and installation instructions.


## Limitations

PlanPro Viewer is still in an early stage of development, and some parts are still missing or do not work properly.
Especially the following limitations have to be taken into account:

- PlanPro Viewer supports input files only in PlanPro version 1.9.0, and only the plain XML files (file extension *.ppxml) can be processed.
  Plans in the zipped container format (file extension *.planpro) are not supported.
- The user interface always contains some parts in German, even if the language is set to English.
  Because PlanPro is a German format with German object and attribute names, this cannot be avoided completely even in future releases.


## Licensing

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
[GNU General Public License](LICENSE) for more details.

The program contains additional third-party components.
Please see the [third-party-licenses.txt](third-party-licenses.txt) file for details.
