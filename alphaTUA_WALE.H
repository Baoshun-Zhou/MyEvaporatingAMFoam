//Get the mesh size
volScalarField V
    (
    IOobject
    (
        mesh.V().name(),
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
    ),
    mesh,
    dimensionedScalar(mesh.V().dimensions(),Zero),
    calculatedFvPatchField<scalar>::typeName
);
V.ref() = mesh.V();
volScalarField delta = pow(V, 1./3);

// Create the variables needed by SGS term
volTensorField gradU(fvc::grad(U));
volSymmTensorField Sd(dev(symm(gradU & gradU)));
volScalarField magSqrSd(magSqr(Sd));
volScalarField nuSGS = 
        sqrt(0.5*delta/1.0)*
            sqrt(
                pow(magSqrSd,3.0)
                /(
                    sqr
                    (     pow(magSqr(symm(gradU)), 5.0/2.0)
                        + pow(magSqrSd, 5.0/4.0)
                    )
                + dimensionedScalar
                (
                    "SMALL",
                    dimensionSet(0,0,-10,0,0),
                    SMALL
                )   
                )
                );

// SGS term
volScalarField TUA(
    nuSGS*fvc::laplacian(alpha1)
);

