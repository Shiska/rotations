stock bool: MatrixMul(Float: A[][], Float: B[][], Float: C[][], m = sizeof A, n = sizeof A[], n2 = sizeof B, o = sizeof B[], m2 = sizeof C, o2 = sizeof C[]) {
    /// <summary>Matrix multiplication, A * B = C</summary>
    /// <param name="A">Matrix multiplicand (m x n)</param>
    /// <param name="B">Matrix multiplier (n x o)</param>
    /// <param name="C">Matrix product (m x o)</param>
    /// <param name="m">Rows of matrix A</param>
    /// <param name="n">Coloums of matrix A</param>
    /// <param name="n2">Rows of matrix B</param>
    /// <param name="o">Coloums of Matrix B</param>
    /// <param name="m2">Rows of matrix C</param>
    /// <param name="o2">Coloums of Matrix C</param>
    /// <returns>true if (n == n2 && m == m2 && p == p2) otherwise false</returns>

    if(m == m2 && n == n2 && o == o2) {
        for(m = 0; m < m2; ++m) { // loop through rows of C
            for(o = 0; o < o2; ++o) { // loop through coloums of C
                for(C[m][o] = 0.0, n = 0; n < n2; ++n) { // row[m] * coloum[o]
                    C[m][o] += A[m][n] * B[n][o];
                }
            }
        }
        return true;
    }
    return false;
}

stock bool: MatrixAdd(Float: A[][], Float: B[][], Float: C[][], mA = sizeof A, nA = sizeof A[], mB = sizeof B, nB = sizeof B[], mC = sizeof C, nC = sizeof C[]) {
    /// <summary>Matrix addition, A + B = C</summary>
    /// <param name="A">Summand (m x n)</param>
    /// <param name="B">Summand (m x n)</param>
    /// <param name="C">Sum (m x n)</param>
    /// <param name="mA">Rows of matrix A</param>
    /// <param name="nA">Coloums of matrix A</param>
    /// <param name="mB">Rows of matrix B</param>
    /// <param name="nB">Coloums of Matrix B</param>
    /// <param name="mC">Rows of matrix C</param>
    /// <param name="nC">Coloums of Matrix C</param>
    /// <returns>true if (mA == mB == mC && nA == nB == nC) otherwise false</returns>

    if(mA == mB && mB == mC && nA == nB && nB == nC) {
        for(mA = 0; mA < mB; ++mA) {
            for(nA = 0; nA < nB; ++nA) {
                C[mA][nA] = A[mA][nA] + B[mA][nA];
            }
        }
        return true;
    }
    return false;
}

stock bool: MatrixSub(Float: A[][], Float: B[][], Float: C[][], mA = sizeof A, nA = sizeof A[], mB = sizeof B, nB = sizeof B[], mC = sizeof C, nC = sizeof C[]) {
    /// <summary>Matrix subtraction, A - B = C</summary>
    /// <param name="A">Subtrahend (m x n)</param>
    /// <param name="B">Minuend (m x n)</param>
    /// <param name="C">Difference (m x n)</param>
    /// <param name="mA">Rows of matrix A</param>
    /// <param name="nA">Coloums of matrix A</param>
    /// <param name="mB">Rows of matrix B</param>
    /// <param name="nB">Coloums of Matrix B</param>
    /// <param name="mC">Rows of matrix C</param>
    /// <param name="nC">Coloums of Matrix C</param>
    /// <returns>true if (mA == mB == mC && nA == nB == nC) otherwise false</returns>

    if(mA == mB && mB == mC && nA == nB && nB == nC) {
        for(mA = 0; mA < mB; ++mA) {
            for(nA = 0; nA < nB; ++nA) {
                C[mA][nA] = A[mA][nA] - B[mA][nA];
            }
        }
        return true;
    }
    return false;
}

stock RotationMatrixX(Float: matrix[4][4], Float: angle) {
    /// <summary>Rotation matrix around X axis matrix</summary>
    /// <param name="matrix">Dest matrix</param>
    /// <param name="angle">Rotation around axis</param>
    /// <remarks>[second Rotation] * [first Rotation] = global frame of reference - global coordinate system / axis stay the same for the second rotation</remarks>
    /// <remarks>[first Rotation] * [second Rotation] = local frame-of-reference - coordinate system / axis rotates with first rotation</remarks>

    matrix[0][0] =
    matrix[3][3] = 1.0;

    matrix[2][2] = matrix[1][1] = floatcos(angle, degrees);
    matrix[1][2] = -(matrix[2][1] = floatsin(angle, degrees));

    matrix[0][1] =
    matrix[0][2] =
    matrix[0][3] =
    matrix[1][0] =
    matrix[1][3] =
    matrix[2][0] =
    matrix[2][3] =
    matrix[3][0] =
    matrix[3][1] =
    matrix[3][2] =
    matrix[3][3] = 0.0;
}

stock RotationMatrixY(Float: matrix[4][4], Float: angle) {
    /// <summary>Rotation matrix around Y axis matrix</summary>
    /// <param name="matrix">Dest matrix</param>
    /// <param name="angle">Rotation around axis</param>
    /// <remarks>[second Rotation] * [first Rotation] = global frame of reference - global coordinate system / axis stay the same for the second rotation</remarks>
    /// <remarks>[first Rotation] * [second Rotation] = local frame-of-reference - coordinate system / axis rotates with first rotation</remarks>

    matrix[1][1] =
    matrix[3][3] = 1.0;

    matrix[2][2] = matrix[0][0] = floatcos(angle, degrees);
    matrix[2][0] = -(matrix[0][2] = floatsin(angle, degrees));

    matrix[0][1] =
    matrix[0][3] =
    matrix[1][0] =
    matrix[1][2] =
    matrix[1][3] =
    matrix[2][1] =
    matrix[2][3] =
    matrix[3][0] =
    matrix[3][1] =
    matrix[3][2] =
    matrix[3][3] = 0.0;
}

stock RotationMatrixZ(Float: matrix[][], Float: angle) {
    /// <summary>Rotation matrix around Z axis matrix</summary>
    /// <param name="matrix">Dest matrix</param>
    /// <param name="angle">Rotation around axis</param>
    /// <remarks>[second Rotation] * [first Rotation] = global frame of reference - global coordinate system / axis stay the same for the second rotation</remarks>
    /// <remarks>[first Rotation] * [second Rotation] = local frame-of-reference - coordinate system / axis rotates with first rotation</remarks>

    matrix[2][2] =
    matrix[3][3] = 1.0;

    matrix[1][1] = matrix[0][0] = floatcos(angle, degrees);
    matrix[0][1] = -(matrix[1][0] = floatsin(angle, degrees));

    matrix[0][2] =
    matrix[0][3] =
    matrix[1][2] =
    matrix[1][3] =
    matrix[2][0] =
    matrix[2][1] =
    matrix[2][3] =
    matrix[3][0] =
    matrix[3][1] =
    matrix[3][2] =
    matrix[3][3] = 0.0;
}

stock TranslationMatrix(Float: matrix[4][4], Float: x, Float: y, Float: z) {
    /// <summary>Translation matrix, translates matrix (T * M - left multiplication) or rotates matrix (M * T - right multiplication)</summary>
    /// <param name="matrix">Dest matrix</param>
    /// <param name="m00">First scale</param>
    /// <param name="m11">Second scale</param>
    /// <param name="m22">Third scale</param>
    /// <param name="m33">Fourth scale</param>
    /// <remarks>T * R translate rotation matrix R</remarks>
    /// <remarks>R * T rotates translation in R</remarks>
    /// <remarks>T * T add translation together</remarks>
    /// <remarks>Typically used in "Rotation about Any Point" with (T * M * T^-1) - T^-1 is T with negative x, y, z, so T * T^-1 = Identity matrix</remarks>

    matrix[0][0] =
    matrix[1][1] =
    matrix[2][2] =
    matrix[3][3] = 1.0;

    matrix[0][3] = x;
    matrix[1][3] = y;
    matrix[2][3] = z;

    matrix[0][1] =
    matrix[0][2] =
    matrix[1][0] =
    matrix[1][2] =
    matrix[2][0] =
    matrix[2][1] =
    matrix[3][0] =
    matrix[3][1] =
    matrix[3][2] = 0.0;
}

stock ScalerMatrix(Float: matrix[4][4], Float: m00, Float: m11, Float: m22, Float: m33) {
    /// <summary>Scalar matrix</summary>
    /// <param name="matrix">Dest matrix</param>
    /// <param name="m00">First scale</param>
    /// <param name="m11">Second scale</param>
    /// <param name="m22">Third scale</param>
    /// <param name="m33">Fourth scale</param>
    /// <remarks>S * M scales rows of M</remarks>
    /// <remarks>M * S scales coloums of M</remarks>

    matrix[0][0] = m00;
    matrix[1][1] = m11;
    matrix[2][2] = m22;
    matrix[3][3] = m33;

    matrix[0][1] =
    matrix[0][2] =
    matrix[0][3] =
    matrix[1][0] =
    matrix[1][2] =
    matrix[1][3] =
    matrix[2][0] =
    matrix[2][1] =
    matrix[2][3] =
    matrix[3][0] =
    matrix[3][1] =
    matrix[3][2] = 0.0;
}