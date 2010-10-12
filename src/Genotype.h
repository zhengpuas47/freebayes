#ifndef __GENOTYPE_H
#define __GENOTYPE_H

#include <iostream>
#include <vector>
#include <utility> // pair
#include <algorithm>
#include <numeric>
#include <vector>
#include <iterator>
#include <math.h>
#include <numeric>
#include "Allele.h"

using namespace std;


// TODO
// Develop this stub to clean up leaky abstraction caused by direct use of
// a pair as a complex datatype.
// It's clearer to write things like 'genotypeelement->allele' and
// 'genotypeelement->count' than ..->first and ..->second
class GenotypeElement {

    friend ostream& operator<<(ostream& out, GenotypeElement& rhs);

public:
    Allele allele;
    int count;
    GenotypeElement(const Allele& a, int c) : allele(a), count(c) { }

};


class Genotype : public vector<GenotypeElement> {

    friend ostream& operator<<(ostream& out, const pair<Allele, int>& rhs);
    friend ostream& operator<<(ostream& out, const Genotype& g);
    friend bool operator<(Genotype& a, Genotype& b);

public:
    
    int ploidy;
    vector<Allele> alleles;

    Genotype(vector<Allele>& ungroupedAlleles) {
        alleles = ungroupedAlleles;
        sort(alleles.begin(), alleles.end());
        vector<vector<Allele> > groups = groupAlleles_copy(alleles);
        for (vector<vector<Allele> >::const_iterator group = groups.begin(); group != groups.end(); ++group) {
            this->push_back(GenotypeElement(group->front(), group->size()));
        }
        ploidy = getPloidy();
    }

    vector<Allele> uniqueAlleles(void);
    int getPloidy(void);
    int alleleFrequency(Allele& allele);
    bool containsAllele(Allele& allele);
    bool containsAlleleOtherThan(string& base);
    vector<Allele> alternateAlleles(string& refbase);
    int alleleCount(string& base);
    // the probability of drawing each allele out of the genotype, ordered by allele
    vector<long double> alleleProbabilities(void);
    string str(void);
    string relativeGenotype(string& refbase, string& altbase);
    bool homozygous(void);
    vector<int> alleleCountsInObservations(vector<Allele*> observations);
    /*
    void inOutObservationCounts(vector<Allele*> observations,
            vector<pair<Allele, int> >& inCounts, 
            vector<pair<Allele, int> >& outCounts);
            */

};

// for sorting pairs of genotype, probs
/*bool genotypeCmp(pair<Genotype, long double> a, pair<Genotype, long double> b) {
    return a.second > b.second;
}
*/

string IUPAC(Genotype& g);
string IUPAC2GenotypeStr(string iupac);

vector<Genotype> allPossibleGenotypes(int ploidy, vector<Allele> potentialAlleles);

typedef vector<pair<string, pair<Genotype*, long double> > > GenotypeCombo;

class GenotypeComboResult {
public:
    GenotypeCombo combo;
    long double comboProb;
    long double probObsGivenGenotypes;
    long double priorProbGenotypeCombo;
    long double priorProbGenotypeComboG_Af;
    long double priorProbGenotypeComboAf;
    GenotypeComboResult(GenotypeCombo& gc,
            long double cp,
            long double pogg,
            long double ppgc,
            long double ppgcgaf,
            long double ppgcaf)
        : combo(gc)
        , comboProb(cp)
        , probObsGivenGenotypes(pogg)
        , priorProbGenotypeCombo(ppgc)
        , priorProbGenotypeComboG_Af(ppgcgaf)
        , priorProbGenotypeComboAf(ppgcaf)
    { }
};

bool genotypeComboResultSorter(const GenotypeComboResult& gc1, const GenotypeComboResult& gc2);

typedef map<string, pair<Genotype*, long double> > GenotypeComboMap;

GenotypeComboMap genotypeCombo2Map(GenotypeCombo& gc);

vector<GenotypeCombo>
bandedGenotypeCombinations(
        vector<pair<string, vector<pair<Genotype*, long double> > > >& sampleGenotypes,
        int bandwidth, int banddepth);

vector<GenotypeCombo>
bandedGenotypeCombinationsIncludingBestHomozygousCombo(
        vector<pair<string, vector<pair<Genotype*, long double> > > >& sampleGenotypes,
        int bandwidth, int banddepth);

vector<GenotypeCombo>
bandedGenotypeCombinationsIncludingAllHomozygousCombos(
    vector<pair<string, vector<pair<Genotype*, long double> > > >& sampleGenotypes,
    vector<Genotype>& genotypes,
    int bandwidth, int banddepth);

bool isHomozygousCombo(GenotypeCombo& combo);
vector<pair<Allele, int> > alternateAlleles(GenotypeCombo& combo, string referenceBase);

pair<int, int> alternateAndReferenceCount(vector<Allele*>& observations, string& refbase, string altbase);

ostream& operator<<(ostream& out, vector<GenotypeCombo>& combo);
ostream& operator<<(ostream& out, GenotypeCombo& g);


#endif
