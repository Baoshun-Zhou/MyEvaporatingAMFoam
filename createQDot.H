Info << "Creating heat flux field qDot\n";

IOdictionary qDotDict
(
    IOobject
    (
        "qDot",
        runTime.constant(), //folder where the fvSolution file is located
        runTime,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    )
);


dictionary& meltingDict = qDotDict.subDict("HeatFlux");

scalar qDot_value(readScalar(meltingDict.lookup("qDot_value")));
scalar xMin(readScalar(meltingDict.lookup("xMin")));
scalar xMax(readScalar(meltingDict.lookup("xMax")));
scalar yMin(readScalar(meltingDict.lookup("yMin")));
scalar yMax(readScalar(meltingDict.lookup("yMax")));
scalar zMin(readScalar(meltingDict.lookup("zMin")));
scalar zMax(readScalar(meltingDict.lookup("zMax")));


volScalarField qDot
(
    IOobject
    (
        "qDot", // Field name
        runTime.timeName(),
        mesh,
        IOobject::NO_READ, // Do not read from disk
        IOobject::AUTO_WRITE // Automatically write when writing fields
    ),
    mesh,
    dimensionedScalar("qDot", dimensionSet(1, 0, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W
    //dimensionedScalar("qDot", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W/m³
);


volScalarField qDot22
(
    IOobject
    (
        "qDot22", // Field name
        runTime.timeName(),
        mesh,
        IOobject::NO_READ, // Do not read from disk
        IOobject::AUTO_WRITE // Automatically write when writing fields
    ),
    mesh,
    dimensionedScalar("qDot22", dimensionSet(1, 0, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W
    //dimensionedScalar("qDot", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W/m³
);



forAll(qDot, cellI)
{
    const vector& C = mesh.C()[cellI];  // Get cell center coordinates

    if (C.x() >= xMin && C.x() <= xMax &&
        C.y() >= yMin && C.y() <= yMax &&
        C.z() >= zMin && C.z() <= zMax)
    {
        qDot[cellI] = qDot_value; // Example: Heat flux of 1 MW/m³
    }
}

volScalarField qDot_interFace
(
    IOobject
    (
        "qDot_interFace", // Field name
        runTime.timeName(),
        mesh,
        IOobject::NO_READ, // Do not read from disk
        IOobject::AUTO_WRITE // Automatically write when writing fields
    ),
    mesh,
    dimensionedScalar("qDot_interFace", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W/m³
);

