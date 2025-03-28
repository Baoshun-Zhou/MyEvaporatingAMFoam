#ifndef HeatSource_H
#define HeatSource_H

#include "volFields.H"
#include "dictionary.H"

namespace Foam
{

class HeatSource
:
public IOdictionary
{
    volScalarField deposition_;

    volScalarField yDim_;

    HeatSource(const HeatSource&);
    
    void operator=(const HeatSource&);


public:

    //TypeName("HeatSource");

    HeatSource
    (
        const fvMesh& mesh
    );

    virtual ~HeatSource()
    {}

    const volScalarField& deposition() const
        {
            return deposition_;
        }

        //- Non-const access to the yDim field
        volScalarField& yDim()
        {
            return yDim_;
        }

        void updateDeposition
        (
            const volScalarField& alphaFiltered,
            const volVectorField& nFiltered
        );

};

}


#endif