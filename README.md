# COMP1000 C1 W2 - Bespoke database using C++

### Background

I can't remember exactly what the specifications were for this project; the brief file no longer exists. It was something along the line of creating a bespoke text-based database system for storing student records using C++.

For each task (4 in total), we had to create a program to fulfil the requirements. Tasks 1 - 3 are command line programs whereas task 4 is a Qt GUI application. The requirements for tasks 1, 2, and 3 are present as comments in their respective `main.cpp` files.

This project ended up receiving a mark of 99 / 100, apparently the highest in the class.

### Highlights

- The `/Core` module contains common code that is reused across tasks to reduce code duplication.
- The database is stored as a simple text file that denotes records and fields using hashtags as per the requirements.
- The code for the database system itself it centralised in the `/Core` module, the task programs provide an interface for interacting with the database.
- The command line programs use a custom-made system to streamline the process of managing arguments and validating inputs.

### Project Structure

I have restructured the project and code structure from the original template. VS solution can be found in the `Application` directory. 

The core database code and some utility classes / functions are stored in the `Application/Core/` directory.

The code for the respective tasks as well as their project files can be found in the `Application/Tasks/` directory.

Built files are output to the `Application/Build/` directory.

### Running with Visual Studio (Tasks 1, 2, and 3)

Open the solution file found in the `Application/` directory.

### Running with Qt (Task 4)

***Important***: When running task 4, make sure you launch using the `CMakeLists.txt` file in the `Application/` directory. Using the CMake file in the task-4 directory will not work.

<!-- Please reads the coursework spec.

If using Visual Studio 2022 (for Tasks A-C), you must use the solution file comp1000C1W2.sln is in the tasks-VS2022 folder.

If using Qt Creator, the project files are in the tasks folder. Click File->Open File or Project, browse to the tasks folder, the into the relevant subfolder, and select the CMakeLists.txt file. -->