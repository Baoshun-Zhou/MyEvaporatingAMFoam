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
    dimensionedScalar("qDot", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W/m³
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



/*
#include "fvCFD.H"
#include "dictionary.H"

Info << "Creating heat flux field qDot\n";

//constant dictionary HeatFlux = readDictionary("constant/qDotDict");

volScalarField p_rgh
(
    IOobject
    (
        "p_rgh",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


volScalarField dict
(
        dictFileName, // Name of your dictionary file (e.g., qDotDict)
        runTime.constant(), // Look for the dictionary in the constant folder
        mesh,
        IOobject::MUST_READ, // Read the dictionary file
        IOobject::AUTO_WRITE  // Automatically write if necessary
);

const scalar xMin = (readScalar(dict.lookup("xMin")));

const scalar xMax(readScalar(dict.lookup("xMax")));
const scalar yMin(readScalar(dict.lookup("yMin")));
const scalar yMax(readScalar(dict.lookup("yMax")));
const scalar zMin(readScalar(dict.lookup("zMin")));
const scalar zMax(readScalar(dict.lookup("zMax")));



//const scalar qdot_value =  HeatFlux.lookup("qdot_value");

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
    dimensionedScalar("qDot", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W/m³
);

forAll(qDot, cellI)
{
    const vector& C = mesh.C()[cellI];  // Get cell center coordinates

     // Define coordinate range where heat flux is applied
    scalar xMin = 0.0002, xMax = 0.0018;
    scalar yMin = 0.0018, yMax = 0.0022;
    scalar zMin = 0.0, zMax = 1.0, qdot_value = 1e10;

    if (C.x() >= xMin && C.x() <= xMax &&
        C.y() >= yMin && C.y() <= yMax &&
        C.z() >= zMin && C.z() <= zMax)
    {
        qDot[cellI] = qdot_value; // Example: Heat flux of 1 MW/m³
    }
}





forAll(qDot, cellI)
{
    const vector& C = mesh.C()[cellI];  // Get cell center coordinates

    // Define coordinate range where heat flux is applied
    //scalar xMin = 0.0002, xMax = 0.0018;
    //scalar yMin = 0.0018, yMax = 0.0022;
    //scalar zMin = 0.0, zMax = 1.0;

    if (C.x() >= xMin && C.x() <= xMax &&
        C.y() >= yMin && C.y() <= yMax &&
        C.z() >= zMin && C.z() <= zMax)
    {
        qDot[cellI] = qdot_value; // Example: Heat flux of 1 MW/m³
    }
}
*/
