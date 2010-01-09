/*
 *
 *
 * COLORES ATOMICOS DE http://jmol.sourceforge.net/jscolors/
 *
 */

#include <lpmd/colorhandler.h>
#include <lpmd/basicatom.h>
#include <lpmd/error.h>

#include <map>

using namespace lpmd;

class lpmd::ColorHandlerImpl
{
 public:
   ColorHandlerImpl() { }

   std::map<unsigned long int, Color> colormap;
};

lpmd::ColorHandlerImpl ColorHandler::impl = ColorHandlerImpl();

ColorHandler::ColorHandler() { }

bool ColorHandler::HaveColor(const BasicAtom & at) { return (impl.colormap.count(at.ID()) > 0); }

Color & ColorHandler::ColorOfAtom(const BasicAtom & at)
{
 try { return impl.colormap[at.ID()]; }
 catch(Error &) { throw RuntimeError("Atom has no color"); }
}

Color ColorHandler::DefaultColor(const BasicAtom & at)
{
 const lpmd::Vector DefCol(0,0.8,0.8);
 lpmd::Vector ans;
 int spc=at.Z();
 //COLORS TAKEN FROM http://jmol.sourceforge.net/jscolors/
 if      (spc==1) ans=(1.000*e1+1.000*e2+1.000*e3);             //Hydrogen (colorless)
 else if (spc==2) ans=(0.851*e1+1.000*e2+1.000*e3);             //Helium (colorless)
 else if (spc==3) ans=(0.800*e1+0.502*e2+1.000*e3);             //Lithium (silvery)
 else if (spc==4) ans=(0.761*e1+1.000*e2+0.000*e3);             //Beryllium (gray)
 else if (spc==5) ans=(1.000*e1+0.710*e2+0.710*e3);             //Boron (brownish)
 else if (spc==6) ans=(0.565*e1+0.565*e2+0.565*e3);             //Carbon (black)
 else if (spc==7) ans=(0.188*e1+0.314*e2+0.973*e3);             //Nitrogen (colorless)
 else if (spc==8) ans=(1.000*e1+0.051*e2+0.051*e3);             //Oxigen (colorless)
 else if (spc==9) ans=(0.565*e1+0.878*e2+0.314*e3);             //Fluorine (Greenish)
 else if (spc==10)ans=(0.702*e1+0.890*e2+0.961*e3);             //Neon (colorless)
 else if (spc==11)ans=(0.671*e1+0.361*e2+0.949*e3);             //Sodium (silvery)
 else if (spc==12)ans=(0.541*e1+1.000*e2+0.000*e3);             //Magnesium (grayish)
 else if (spc==13)ans=(0.749*e1+0.651*e2+0.651*e3);             //Aluminum  (silver)
 else if (spc==14)ans=(0.941*e1+0.784*e2+0.623*e3);             //Silicon   (gray)
 else if (spc==15)ans=(1.000*e1+0.502*e2+0.000*e3);             //Phosphorus (white)
 else if (spc==16)ans=(1.000*e1+1.000*e2+0.188*e3);             //Sulfur (yellow)
 else if (spc==17)ans=(0.122*e1+0.941*e2+0.122*e3);             //Chlorine (green)
 else if (spc==18)ans=(0.502*e1+0.820*e2+0.890*e3);             //Argon  (colorless)
 else if (spc==19)ans=(0.561*e1+0.251*e2+0.831*e3);             //Potassium (silvery)
 else if (spc==20)ans=(0.239*e1+1.000*e2+0.000*e3);             //Calcium (silvery)
 else if (spc==21)ans=(0.902*e1+0.902*e2+0.902*e3);             //Scandium (silvery)
 else if (spc==22)ans=(0.749*e1+0.761*e2+0.780*e3);             //Titanium (silverish)
 else if (spc==23)ans=(0.651*e1+0.651*e2+0.671*e3);             //Vanadium (silverish)
 else if (spc==24)ans=(0.541*e1+0.600*e2+0.780*e3);             //Chromium (gray)
 else if (spc==25)ans=(0.611*e1+0.478*e2+0.780*e3);             //Manganese (silverish)
 else if (spc==26)ans=(0.878*e1+0.400*e2+0.200*e3);             //Iron (silvery)
 else if (spc==27)ans=(0.941*e1+0.565*e2+0.627*e3);             //Cobalt (silvery)
 else if (spc==28)ans=(0.314*e1+0.815*e2+0.314*e3);             //Nickel (white)
 else if (spc==29)ans=(0.784*e1+0.502*e2+0.200*e3);             //Copper (red/orange)
 else if (spc==30)ans=(0.490*e1+0.502*e2+0.690*e3);             //Zinc (bluish)
 else if (spc==31)ans=(0.761*e1+0.561*e2+0.561*e3);             //Gallium (White/Silver)
 else if (spc==32)ans=(0.400*e1+0.561*e2+0.561*e3);             //Germanium (grayish)
 else if (spc==33)ans=(0.741*e1+0.502*e2+0.890*e3);             //Arsenic (Gray)
 else if (spc==34)ans=(1.000*e1+0.631*e2+0.000*e3);             //Selenium (Gray)
 else if (spc==35)ans=(0.651*e1+0.161*e2+0.161*e3);             //Bromine (Red)
 else if (spc==36)ans=(0.361*e1+0.722*e2+0.820*e3);             //Krypton (colorless)
 else if (spc==37)ans=(0.439*e1+0.180*e2+0.690*e3);             //Rubidium (silver)
 else if (spc==38)ans=(0.000*e1+1.000*e2+0.000*e3);             //Strontium (yellowish)
 else if (spc==39)ans=(0.580*e1+1.000*e2+1.000*e3);             //Yttrium (silvery)
 else if (spc==40)ans=(0.580*e1+0.878*e2+0.878*e3);             //Zirconium (Grayish)
 else if (spc==41)ans=(0.451*e1+0.761*e2+0.788*e3);             //Niobium (white)
 else if (spc==42)ans=(0.329*e1+0.710*e2+0.710*e3);             //Molybdenum (silverish)
 else if (spc==43)ans=(0.231*e1+0.620*e2+0.620*e3);             //Technetium (unknown)
 else if (spc==44)ans=(0.141*e1+0.561*e2+0.561*e3);             //Ruthenium (silvery)
 else if (spc==45)ans=(0.039*e1+0.490*e2+0.549*e3);             //Rhodium (silverish)
 else if (spc==46)ans=(0.000*e1+0.412*e2+0.522*e3);             //Palladium (white)
 else if (spc==47)ans=(0.753*e1+0.753*e2+0.753*e3);             //Silver (silver)
 else if (spc==48)ans=(1.000*e1+0.851*e2+0.561*e3);             //Cadmium (silver)
 else if (spc==49)ans=(0.651*e1+0.459*e2+0.451*e3);             //Indium (silver)
 else if (spc==50)ans=(0.400*e1+0.502*e2+0.502*e3);             //Tin (white)
 else if (spc==51)ans=(0.620*e1+0.388*e2+0.710*e3);             //Antimony (bluish)
 else if (spc==52)ans=(0.831*e1+0.478*e2+0.000*e3);             //Tellurium (silverish)
 else if (spc==53)ans=(0.580*e1+0.000*e2+0.580*e3);             //Iodine (blackish)
 else if (spc==54)ans=(0.259*e1+0.620*e2+0.690*e3);             //Xenon  (colorless)
 else if (spc==55)ans=(0.341*e1+0.090*e2+0.561*e3);             //Cesium  (silver)
 else if (spc==56)ans=(0.000*e1+0.788*e2+0.000*e3);             //Barium  (silver)
 else if (spc==57)ans=(0.439*e1+0.831*e2+1.000*e3);             //Lanthanum
 else if (spc==58)ans=(1.000*e1+1.000*e2+0.780*e3);             //Cerium
 else if (spc==59)ans=(0.851*e1+1.000*e2+0.780*e3);             //Praseodymium
 else if (spc==60)ans=(0.780*e1+1.000*e2+0.780*e3);             //Neodymium
 else if (spc==61)ans=(0.639*e1+1.000*e2+0.780*e3);             //Promethium
 else if (spc==62)ans=(0.561*e1+1.000*e2+0.780*e3);             //Samarium
 else if (spc==63)ans=(0.380*e1+1.000*e2+0.780*e3);             //Europium
 else if (spc==64)ans=(0.271*e1+1.000*e2+0.780*e3);             //Gadolinium
 else if (spc==65)ans=(0.188*e1+1.000*e2+0.780*e3);             //Terbium
 else if (spc==66)ans=(0.122*e1+1.000*e2+0.780*e3);             //Dysprosium
 else if (spc==67)ans=(0.000*e1+1.000*e2+0.612*e3);             //Holmium
 else if (spc==68)ans=(0.000*e1+0.902*e2+0.459*e3);             //Erbium
 else if (spc==69)ans=(0.000*e1+0.831*e2+0.322*e3);             //Thulium
 else if (spc==70)ans=(0.000*e1+0.749*e2+0.219*e3);             //Ytterbium
 else if (spc==71)ans=(0.000*e1+0.671*e2+0.141*e3);             //Lutetium
 else if (spc==72)ans=(0.302*e1+0.761*e2+1.000*e3);             //Hafnium  (silver)
 else if (spc==73)ans=(0.302*e1+0.651*e2+1.000*e3);             //Tantalum  (gray)
 else if (spc==74)ans=(0.129*e1+0.580*e2+0.839*e3);             //Tungsten (silver)
 else if (spc==75)ans=(0.149*e1+0.490*e2+0.671*e3);             //Rhenium (silver)
 else if (spc==76)ans=(0.149*e1+0.400*e2+0.588*e3);             //Osmium (silver)
 else if (spc==77)ans=(0.090*e1+0.329*e2+0.529*e3);             //Iridium (white)
 else if (spc==78)ans=(0.816*e1+0.816*e2+0.878*e3);             //Platinum (silverish)
 else if (spc==79)ans=(1.000*e1+0.820*e2+0.137*e3);             //Gold (gold)
 else if (spc==80)ans=(0.722*e1+0.722*e2+0.816*e3);             //Mercury (silver)
 else if (spc==81)ans=(0.651*e1+0.329*e2+0.302*e3);             //Thallium (bluish)
 else if (spc==82)ans=(0.341*e1+0.349*e2+0.380*e3);             //Lead (bluish)
 else if (spc==83)ans=(0.620*e1+0.310*e2+0.710*e3);             //Bismuth (white)
 else if (spc==84)ans=(0.671*e1+0.361*e2+0.000*e3);             //Polonium (unknown)
 else if (spc==85)ans=(0.459*e1+0.310*e2+0.271*e3);             //Astatine (unknown)
 else if (spc==86)ans=(0.259*e1+0.510*e2+0.588*e3);             //Radon (colorless)
 else if (spc==87)ans=(0.259*e1+0.000*e2+0.400*e3);             //Francium (unknown)
 else if (spc==88)ans=(0.000*e1+0.490*e2+0.000*e3);             //Radium (silverish)
 else if (spc==89)ans=(0.439*e1+0.671*e2+0.980*e3);             //Actinium
 else if (spc==90)ans=(0.000*e1+0.729*e2+1.000*e3);             //Thorium
 else if (spc==91)ans=(0.000*e1+0.631*e2+1.000*e3);             //Protactinium
 else if (spc==92)ans=(0.000*e1+0.561*e2+1.000*e3);             //Uranium
 else if (spc==93)ans=(0.000*e1+0.502*e2+1.000*e3);             //Neptunium
 else if (spc==94)ans=(0.000*e1+0.402*e2+1.000*e3);             //Plutonium
 else if (spc==95)ans=(0.329*e1+0.361*e2+0.949*e3);             //Americium
 else if (spc==96)ans=(0.471*e1+0.361*e2+0.890*e3);             //Curium
 else if (spc==97)ans=(0.541*e1+0.310*e2+0.890*e3);             //Berkelium
 else if (spc==98)ans=(0.631*e1+0.212*e2+0.831*e3);             //Californium
 else if (spc==99)ans=(0.702*e1+0.122*e2+0.831*e3);             //Einstenium
 else if (spc==100)ans=(0.702*e1+0.122*e2+0.792*e3);            //Fermium
 else if (spc==101)ans=(0.702*e1+0.051*e2+0.651*e3);            //Mendelevium
 else if (spc==102)ans=(0.741*e1+0.051*e2+0.529*e3);            //Nobelium
 else if (spc==103)ans=(0.780*e1+0.000*e2+0.400*e3);            //Lawrencium
 else if (spc==104)ans=(0.800*e1+0.000*e2+0.349*e3);            //Rutherfordium (unknown)
 else if (spc==105)ans=(0.820*e1+0.000*e2+0.310*e3);            //Dubnium (unknown)
 else if (spc==106)ans=(0.851*e1+0.000*e2+0.271*e3);            //Seaborgium (unknown)
 else if (spc==107)ans=(0.878*e1+0.000*e2+0.220*e3);            //Bohrium (unknown)
 else if (spc==108)ans=(0.902*e1+0.000*e2+0.180*e3);            //Hassium (unknown)
 else if (spc==109)ans=(0.922*e1+0.000*e2+0.149*e3);            //Meitnerium (unknown)
 else if (spc==110)ans=DefCol;                                  //Ununnilium (unknown)
 else if (spc==111)ans=DefCol;                                  //Unununium (unknown)
 else if (spc==112)ans=DefCol;                                  //Ununbium (unknown)
 else if (spc==113)ans=DefCol;                                  //Ununtritium (unknown)
 else if (spc==114)ans=DefCol;                                  //Ununquadium (unknown)
 else if (spc==115)ans=DefCol;                                  //Ununpentium (unknown)
 else if (spc==116)ans=DefCol;                                  //Ununhexium (unknown)
 else if (spc==117)ans=DefCol;                                  //Ununseptium (unknown)
 else if (spc==118)ans=DefCol;                                  //Ununoctium (unknown)
 else	           ans=DefCol;
 return ans;
}

