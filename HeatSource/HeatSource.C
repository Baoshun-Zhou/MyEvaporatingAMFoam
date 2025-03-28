#include "HeatSource.H"
#include "fvc.H"
#include "fvCFD.H"
#include "constants.H"
//#include "findLocalCell.H"
#include "SortableList.H"
#include <cmath>

namespace Foam
{
    // This should be in the .C file, not .H
    //defineTypeNameAndDebug(HeatSource, 0);

    // Constructor definition
    HeatSource::HeatSource(const fvMesh &mesh)
        : IOdictionary(
              IOobject(
                  "HeatSource",
                  mesh.time().constant(),
                  mesh,
                  IOobject::MUST_READ,
                  IOobject::NO_WRITE)),
          deposition_(
              IOobject(
                  "deposition",
                  mesh.time().timeName(),
                  mesh,
                  IOobject::NO_READ,
                  IOobject::AUTO_WRITE),
              mesh,
              dimensionedScalar("deposition", dimensionSet(1, -1, -3, 0, 0), -1.0)),
          yDim_(
              IOobject(
                  "yDim",
                  mesh.time().timeName(),
                  mesh,
                  IOobject::NO_READ,
                  IOobject::NO_WRITE),
              mesh,
              dimensionedScalar("yDim", dimensionSet(0, 1, 0, 0, 0), 1.0))
    {
        // Constructor body (optional)
    }

    // Define the updateDeposition method here
    void HeatSource::updateDeposition(const volScalarField &alphaFiltered, const volVectorField &nFiltered)
    {
        deposition_ *= 0.0;

        // Read laser properties and settings
        const fvMesh &mesh = deposition_.mesh();
        const Time &runTime = mesh.time();
        const dimensionedScalar time = runTime.time();
        const scalar HS_bg(readScalar(lookup("HS_bg")));
        const scalar HS_lg(readScalar(lookup("HS_lg")));
        const scalar HS_Q(readScalar(lookup("HS_Q")));

        const dimensionedScalar pi = constant::mathematical::pi;
        const dimensionedScalar b_g("b_g", dimensionSet(0, 1, 0, 0, 0), HS_bg);
        const dimensionedScalar Q_cond("Q_cond", dimensionSet(1, 2, -3, 0, 0), HS_Q);
        const dimensionedScalar lg("lg", dimensionSet(0, 1, 0, 0, 0), HS_lg);

        const vectorField &CI = mesh.C();
        const scalarField &yDimI = yDim_;
        const vectorField &nFilteredI = nFiltered;
        const scalarField &alphaFilteredI = alphaFiltered;

        DynamicList<vector> initial_points;

        List<pointField> gatheredData1(Pstream::nProcs());

        forAll(CI, celli)
        {
            const scalar x_coord = CI[celli].x();
            const scalar y_coord = CI[celli].y();
            const scalar z_coord = CI[celli].z();  // Fix: Define z_coord

            scalar x_min(0), y_min(0.0018), z_min(0), x_max(0.002), y_max(0.0022), z_max(1);
            if (x_coord >= x_min && x_coord <= x_max &&
                y_coord >= y_min && y_coord <= y_max &&
                z_coord >= z_min && z_coord <= z_max)
            {
                point p_1(CI[celli].x(), CI[celli].y(), CI[celli].z());
                initial_points.append(p_1);
            }
        }

        gatheredData1[Pstream::myProcNo()] = initial_points;
        Pstream::gatherList(gatheredData1);
        Pstream::scatterList(gatheredData1);

        pointField pointslistGlobal1(
            ListListOps::combine<Field<vector>>(
                gatheredData1,
                accessOp<Field<vector>>()));

        forAll(pointslistGlobal1, i)
        {
            point V1_tip(pointslistGlobal1[i]);

            label myCellId = mesh.findCell(V1_tip);

            if (myCellId >= 0)  // Fix: Check if myCellId is valid before using
            {
                deposition_[myCellId] = Q_cond.value();
            }
        }
    }
}
