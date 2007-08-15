//Frame.cpp


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Frame.h"


Frame::Frame(const int& rAge, 
             const float& rSite, 
             const int& rLastBurn, 
             const float& rFireIgnitionFactor, 
             const float& rFireSensitivity, 
             const Species& rSpeciesSubCanopy)
    : Fire(rLastBurn, rFireIgnitionFactor, rFireSensitivity), _yearEstablished(rAge), _yearFrameEstablished(rAge), _site(rSite), _speciesSubCanopy(rSpeciesSubCanopy)
{
}


//Frame::Frame(const Frame &Frame, const int &nAge) : 
Frame::Frame(const Frame &Frame) 
    : Fire(Frame), _yearEstablished(Frame._yearEstablished), _yearFrameEstablished(Frame._yearFrameEstablished), _site(Frame._site), _speciesSubCanopy(Frame.type())
{ 
}


void Frame::clear()
{
}


std::ostream& operator<< (std::ostream& s, const Frame& rFram) 
{
	int bFormat = rFram._outFlags & outFormat;
	if (rFram._outFlags & outVeg)			{if (!bFormat) s<<"Veg="<<  rFram.type()                        <<"\t";         else s<<    rFram.type()                        <<" "; }
	if (rFram._outFlags & outAge)			{if (!bFormat) s<<"Age="<<  gYear-rFram._yearEstablished        <<"\t";			else s<<    gYear-rFram._yearEstablished        <<" "; }
	if (rFram._outFlags & outSite)		    {if (!bFormat) s<<"Site="<< rFram._site                         <<"\t";         else s<<    rFram._site                         <<" "; }
	if (rFram._outFlags & outSub)			{if (!bFormat) s<<"Sub="<<  rFram._speciesSubCanopy             <<"\t";         else s<<    rFram._speciesSubCanopy             <<" "; }
	if (rFram._outFlags & outFireAge)		{if (!bFormat) s<<"FAge="<< rFram.yearOfLastBurn                <<"\t";         else s<<    rFram.yearOfLastBurn                <<" "; }
	if (rFram._outFlags & outFireScar)	    {if (!bFormat) s<<"FScar=";	s << (rFram.lastBurnWasOrigin?"-":"") << rFram.yearOfLastBurn << "." << rFram.fireScarID;	if (!bFormat) s << "\t"; else s << " ";	}
	rFram.writeData (s);
	return s;
}


void Frame::writeData (std::ostream& s) const 
//A default function for outputing specific frame data.  This function always outputs 0 after
//a NoData label to indicate that there was no output routine specified
{
	if ((_outFlags & out1) || (_outFlags & out2) || (_outFlags & out3) || (_outFlags & out4)) {
	    int formatting = _outFlags & outFormat;
		if (!formatting)
			s << "NoData=" << 0. << "\t";
        else
            s << 0. << " ";
	}
}
