# AOI
Automated Optical Inspection (AOI) System

The system has two core functions: Editing and Inspection.

Editing:
Place a standard PCB on the control console, define the coordinates and inspection areas to be detected, capture images of the designated areas on the standard PCB as reference images, and save both the reference images and their corresponding coordinates.

Inspection:
Move the PCB to the specified positions based on the pre-defined coordinates, control the camera to capture images of the inspection areas, compare them with the reference images at the same coordinates, evaluate the soldering quality within the inspected areas, and output a pass/fail conclusion.

Note:
The system utilizes proprietary components, including stepper motor drivers, camera drivers, and a custom image comparison algorithm. Potential users intending to adopt this system may need to replace these components with their own drivers or algorithms.
