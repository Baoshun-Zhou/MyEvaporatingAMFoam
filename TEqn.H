{
    tmp<volScalarField> tcp(thermo->Cp());
    const volScalarField &cp = tcp();

    const dimensionedScalar Cp1 = thermo->Cp1();
    const dimensionedScalar Cp2 = thermo->Cp2();

    rhoCp = rho * cp;

    kappaEff = thermo->kappa() + rho * cp * turbulence->nut() / Prt;

    const surfaceScalarField rhoCpPhi(
        "rhoCpPhi",
        rhoPhi * (Cp1 - Cp2) + phi * rho2 * Cp2);

    forAll(mesh.C(), celli)
    {

        thermalDamper[celli] = 2.0 * rhoCp[celli] / (rho1.value() * Cp1.value() + rho2.value() * Cp2.value());
    }

    const volVectorField gradAlpha(fvc::grad(alpha1));


    forAll(qDot_interFace, cellI)
    {

        qDot_interFace[cellI] = qDot[cellI] * mag(gradAlpha[cellI]) * thermalDamper[cellI]; // Example: Heat flux of 1 MW/m³
    }

    
    fvScalarMatrix TEqn(
        fvm::ddt(rhoCp, T) + fvm::div(rhoCpPhi, T) - fvm::Sp(fvc::ddt(rhoCp) + fvc::div(rhoCpPhi), T) - fvm::laplacian(kappaEff, T) + mixture->TSource() - qDot * mag(gradAlpha) * thermalDamper);

    TEqn.relax();
    TEqn.solve();

    Info << "min/max(T) = " << min(T).value() << ", "
         << max(T).value() << endl;

    // Info << "fvm::laplacian(kappaEff, T)" << min(fvm::laplacian(kappaEff, T)).value() << ", " << max(fvm::laplacian(kappaEff, T)).value() << endl;
    Info << "min/max(qDot)" << min(qDot).value() << ", " << max(qDot).value() << endl;
}
