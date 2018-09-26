# rotations

[![sampctl](https://shields.southcla.ws/badge/sampctl-rotations-2f2f2f.svg?style=for-the-badge)](https://github.com/Shiska/rotations)

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

This include gives you the possibility to convert rotations

## Installation

Simply install to your project:

```bash
sampctl package install Shiska/rotations
```

Include in your code and begin using the library:

```pawn
#include <rotations>
```

## Functions

<!--
Write your code documentation or examples here. If your library is documented in
the source code, direct users there. If not, list your API and describe it well
in this section. If your library is passive and has no API, simply omit this
section.
-->

RotMatrix...AroundRelPoint
```
RotMatrixMatrixAroundRelPoint(Float: matrix1[4][4], Float: oX, Float: oY, Float: oZ, Float: matrix2[4][4])
RotMatrixQuatAroundRelPoint(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: w, Float: x, Float: y, Float: z)
RotMatrixEulerAroundRelPoint(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
RotMatrixAxisAroundRelPoint(Float: matrix1[4][4], Float: oX, Float: oY, Float: oZ, Float: angle, Float: aX, Float: aY, Float: aZ)
```
RotMatrix...AroundPoint
```
RotMatrixMatrixAroundPoint(Float: matrix1[4][4], Float: x, Float: y, Float: z, Float: matrix2[4][4])
RotMatrixQuatAroundPoint(Float: matrix[4][4], Float: x, Float: y, Float: z, Float: w, Float: qX, Float: qY, Float: qZ)
RotMatrixEulerAroundPoint(Float: matrix[4][4], Float: x, Float: y, Float: z, Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
RotMatrixAxisAroundPoint(Float: matrix1[4][4], Float: x, Float: y, Float: z, Float: angle, Float: aX, Float: aY, Float: aZ)
```
TranslateMatrix
```
TranslateMatrix(Float: matrix[4][4], Float: x, Float: y, Float: z)
```
RotateMatrix...
```
RotateMatrixWithMatrix(Float: matrix1[4][4], Float: matrix2[4][4])
RotateMatrixWithQuat(Float: matrix[4][4], Float: w, Float: x, Float: y, Float: z)
RotateMatrixWithEuler(Float: matrix[4][4], Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
RotateMatrixWithAxisAngle(Float: matrix[4][4], Float: angle, Float: aX, Float: aY, Float: aZ)
```
GetTranslationMatrix
```
GetTranslationMatrix(Float: matrix[4][4], Float: x, Float: y, Float: z)
```
GetRotationMatrix...
```
GetRotationMatrixFromQuat(Float: matrix[4][4], Float: w, Float: x, Float: y, Float: z)
GetRotationMatrixFromEuler(Float: matrix[4][4], Float: rX, Float: rY, Float: rZ, eulermode: mode = euler_samp)
GetRotationMatrixFromAxisAngle(Float: matrix[4][4], Float: angle, Float: aX, Float: aY, Float: aZ)
```
GetQuat...
```
GetQuatFromMatrix(Float: matrix[][], & Float: w, & Float: x, & Float: y, & Float: z)
GetQuatFromEuler(Float: rX, Float: rY, Float: rZ, & Float: w, & Float: x, & Float: y, & Float: z, eulermode: mode = euler_samp)
GetQuatFromAxisAngle(Float: angle, Float: aX, Float: aY, Float: aZ, & Float: w, & Float: x, & Float: y, & Float: z)
```
GetEuler...
```
GetEulerFromMatrix(Float: matrix[][], & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
GetEulerFromQuat(Float: w, Float: x, Float: y, Float: z, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
GetEulerFromEuler(Float: oX, Float: oY, Float: oZ, eulermode: omode, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
GetEulerFromAxisAngle(Float: angle, Float: aX, Float: aY, Float: aZ, & Float: rX, & Float: rY, & Float: rZ, eulermode: mode = euler_samp)
```
GetAxisAngle...
```
GetAxisAngleFromMatrix(Float: matrix[][], & Float: angle, & Float: aX, & Float: aY, & Float: aZ)
GetAxisAngleFromQuat(Float: w, Float: x, Float: y, Float: z, & Float: angle, & Float: aX, & Float: aY, & Float: aZ)
GetAxisAngleFromEuler(Float: rX, Float: rY, Float: rZ, & Float: angle, & Float: aX, & Float: aY, & Float: aZ, eulermode: mode = euler_samp)
```
...Rotate
 ```
MatrixRotate(Float: matrix[4][4], Float: oX, Float: oY, Float: oZ, Float: oT, & Float: X, & Float: Y, & Float: Z)
QuatRotate(Float: w, Float: x, Float: y, Float: z, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z)
EulerRotate(Float: rX, Float: rY, Float: rZ, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z, eulermode: mode = euler_samp)
AxisAngleRotate(Float: angle, Float: aX, Float: aY, Float: aZ, Float: oX, Float: oY, Float: oZ, & Float: X, & Float: Y, & Float: Z)
```

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```
