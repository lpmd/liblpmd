/*
 *
 *
 *
 */

#include <lpmd/atomselector.h>
#include <lpmd/refparticleset.h>

using namespace lpmd;

class ZAtomSelector: public Selector<BasicParticleSet>
{
 public:
   ZAtomSelector(int z): zsel(z) { }

   const BasicParticleSet & SelectFrom(const BasicParticleSet & ps)
   {
    innerps.Clear();
    for (long int i=0;i<ps.Size();++i) 
      if (ps[i].Z() == zsel) innerps.Append(ps[i]);
    return innerps;
   }

   const BasicParticleSet & InverseSelectFrom(const BasicParticleSet & ps) 
   { 
    innerps.Clear();
    for (long int i=0;i<ps.Size();++i) 
      if (ps[i].Z() != zsel) innerps.Append(ps[i]);
    return innerps;
   }
 
 private:
   int zsel;
   RefParticleSet innerps;
};

class TagAtomSelector: public Selector<BasicParticleSet>
{
 public:
   TagAtomSelector(Tag t): tsel(t) { }

   const BasicParticleSet & SelectFrom(const BasicParticleSet & ps) 
   { 
    innerps.Clear();
    for (long int i=0;i<ps.Size();++i) 
      if (ps.Have(ps[i], tsel)) innerps.Append(ps[i]);
    return innerps;
   }

   const BasicParticleSet & InverseSelectFrom(const BasicParticleSet & ps) 
   { 
    innerps.Clear();
    for (long int i=0;i<ps.Size();++i) 
      if (!ps.Have(ps[i], tsel)) innerps.Append(ps[i]);
    return innerps;
   }

 private:
   Tag tsel;
   RefParticleSet innerps;
};

class RegionAtomSelector: public Selector<BasicParticleSet>
{
 public:
   RegionAtomSelector(const Region & r): region(r) { }

   const BasicParticleSet & SelectFrom(const BasicParticleSet & ps) 
   { 
    innerps.Clear();
    for (long int i=0;i<ps.Size();++i) 
      if (region.IsInside(ps[i].Position())) innerps.Append(ps[i]);
    return innerps;
   }

   const BasicParticleSet & InverseSelectFrom(const BasicParticleSet & ps) 
   { 
    innerps.Clear();
    for (long int i=0;i<ps.Size();++i) 
      if (!region.IsInside(ps[i].Position())) innerps.Append(ps[i]);
    return innerps;
   }

 private:
   const Region & region;
   RefParticleSet innerps;
};

//
//
//

class lpmd::AtomSelectorImpl
{
 public:
   Array<Selector<BasicParticleSet> *> s_array;
};

//
//
//

lpmd::AtomSelectorImpl AtomSelector::impl = AtomSelectorImpl();

AtomSelector::AtomSelector() { }

AtomSelector::~AtomSelector() 
{ 
 for (long int i=0;i<impl.s_array.Size();++i) delete impl.s_array[i];
}

Selector<BasicParticleSet> & AtomSelector::WithZ(int z)
{
 Selector<BasicParticleSet> * selector = new ZAtomSelector(z);
 impl.s_array.Append(selector);
 return (*selector);
}

Selector<BasicParticleSet> & AtomSelector::WithTag(Tag t)
{
 Selector<BasicParticleSet> * selector = new TagAtomSelector(t);
 impl.s_array.Append(selector);
 return (*selector);
}

Selector<BasicParticleSet> & AtomSelector::Inside(const Region & r)
{
 Selector<BasicParticleSet> * selector = new RegionAtomSelector(r);
 impl.s_array.Append(selector);
 return (*selector);
}

