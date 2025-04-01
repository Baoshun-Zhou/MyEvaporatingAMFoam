Info << "Creating heat flux field qDot\n";
#include <cmath>
//#include "createFields.H"

IOdictionary qDotDict(
    IOobject(
        "qDot",
        runTime.constant(), // folder where the fvSolution file is located
        runTime,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE));

dictionary &meltingDict = qDotDict.subDict("HeatFlux");

scalar qDot_value(readScalar(meltingDict.lookup("qDot_value")));
scalar x_center(readScalar(meltingDict.lookup("x_center")));
scalar y_center(readScalar(meltingDict.lookup("y_center")));
scalar Radius_Flavour(readScalar(meltingDict.lookup("Radius_Flavour")));
scalar radius(readScalar(meltingDict.lookup("radius")));
scalar absorptivity(readScalar(meltingDict.lookup("absorptivity")));
scalar dim(readScalar(meltingDict.lookup("dim")));

volScalarField qDot(
    IOobject(
        "qDot", // Field name
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,   // Do not read from disk
        IOobject::AUTO_WRITE // Automatically write when writing fields
        ),
    mesh,
    dimensionedScalar("qDot", dimensionSet(1, 0, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W
    // dimensionedScalar("qDot", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W/m³
);


volScalarField dist(
    IOobject(
        "dist", // Field name
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,   // Do not read from disk
        IOobject::AUTO_WRITE // Automatically write when writing fields
        ),
    mesh,
    dimensionedScalar("dist", dimensionSet(1, 0, 0, 0, 0, 0, 0), 0.0) // Default value = 0 W
    // dimensionedScalar("qDot", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0.0) // Default value = 0 W/m³
);


forAll(qDot, cellI)
{
    const scalar x_coord = mesh.C()[cellI].x(); // Get cell center coordinates
    const scalar y_coord = mesh.C()[cellI].y();
    //const scalar z_coord = mesh.C()[cellI].z();
    if (dim == 2)
    {
        dist[cellI] = Foam::sqrt(Foam::pow(x_coord - x_center, 2.0));
    }
    else if(dim == 3)
    {
        dist[cellI] = Foam::sqrt(Foam::pow(x_coord - x_center, 2.0) + Foam::pow(y_coord - y_center, 2.0) );
    }

    qDot[cellI] = absorptivity * Radius_Flavour * qDot_value / ( M_PI * radius *radius) * Foam::exp(-Radius_Flavour * (dist[cellI] /radius)*(dist[cellI] /radius));
}

