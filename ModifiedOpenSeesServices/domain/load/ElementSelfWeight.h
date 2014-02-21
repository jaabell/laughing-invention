#ifndef ElementSelfWeight_h
#define ElementSelfWeight_h

#include <ElementalLoad.h>

class ElementSelfWeight : public ElementalLoad
{
    public:
        ElementSelfWeight(int tag, const ID& theElementTags, const Vector& accelerationfield);

        ElementSelfWeight();
        ~ElementSelfWeight();

        const Vector& getData(int& type, double loadFactor);

        int sendSelf(int commitTag, Channel& theChannel);
        int recvSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker);
        void Print(ostream& s, int flag = 0);


    private:

        Vector data;
};

#endif


