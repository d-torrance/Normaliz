/*
 * Normaliz
 * Copyright (C) 2007-2022  W. Bruns, B. Ichim, Ch. Soeger, U. v. d. Ohe
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * As an exception, when this program is distributed through (i) the App Store
 * by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or (iii) Google Play
 * by Google Inc., then that store may impose any digital rights management,
 * device limits and/or redistribution restrictions that are required by its
 * terms of service.
 */

/**
 * The class Sublattice_Representation represents a sublattice of Z^n as Z^r.
 * To transform vectors of the sublattice  use:
 *    Z^r --> Z^n    and    Z^n -->  Z^r
 *     v  |-> vA             u  |-> (uB)/c
 * A  r x n matrix
 * B  n x r matrix
 * c  Integer
 *
 * The composition of the maps from Z^r to Z^r is c times the 9identity.
 */

#ifndef LIBNORMALIZ_SUBLATTICE_REPRESENTATION_H
#define LIBNORMALIZ_SUBLATTICE_REPRESENTATION_H

#include <vector>

#include <libnormaliz/general.h>
#include <libnormaliz/matrix.h>

//---------------------------------------------------------------------------

namespace libnormaliz {

template <typename Integer>
class Matrix;
// template<typename Integer> class Lineare_Transformation;
using std::vector;

template <typename Integer>
class Sublattice_Representation {
    template <typename>
    friend class Sublattice_Representation;
    template <typename, typename>
    friend class ProjectAndLift;

    size_t dim, rank;
    bool is_identity;
    bool B_is_projection;
    Matrix<Integer> A;
    Matrix<Integer> B;
    Integer c;
    mutable mpz_class external_index;
    mutable Matrix<Integer> Equations;
    mutable bool Equations_computed;
    mutable Matrix<Integer> Congruences;
    mutable bool Congruences_computed;

    vector<key_t> projection_key;

    void make_equations() const;
    void make_congruences() const;

    //---------------------------------------------------------------------------
   public:
    //---------------------------------------------------------------------------
    //	             Construction and destruction
    //---------------------------------------------------------------------------

    /**
     * creates a dummy object
     */
    Sublattice_Representation() {
    }

    /*
        creates a representation of Z^n as a sublattice of itself
    */
    Sublattice_Representation(size_t n);

    // creation from given maps and c
    Sublattice_Representation(const Matrix<Integer>& GivenA, const Matrix<Integer>& GivenB, Integer GivenC);

    /**
     * Main Constructor
     * creates a representation of a sublattice of Z^n
     * if direct_summand is false the sublattice is generated by the rows of M
     * otherwise it is a direct summand of Z^n containing the rows of M
     */
    Sublattice_Representation(const Matrix<Integer>& M, bool take_saturation, bool use_LLL = true);

    template <typename IntegerFC>
    Sublattice_Representation(const Sublattice_Representation<IntegerFC>& Original);
    //---------------------------------------------------------------------------
    //                       Manipulation operations
    //---------------------------------------------------------------------------

    /* workhorse for main constructor */
    void initialize(const Matrix<Integer>& M, bool take_saturation, bool& success);

    //
    // applies LLL reduction to the rows of B and composes *this with the resulting transformation
    void LLL_improve();

    /* first this then SR when going from Z^n to Z^r */
    void compose(const Sublattice_Representation<Integer>& SR);

    /* compose with the dual of SR */
    void compose_dual(const Sublattice_Representation<Integer>& SR);

    void compose_with_passage_to_quotient(Matrix<Integer>& Sub, Matrix<Integer>& Perp);

    //---------------------------------------------------------------------------
    //                       Transformations
    //---------------------------------------------------------------------------

    Matrix<Integer> to_sublattice(const Matrix<Integer>& M) const;
    Matrix<Integer> from_sublattice(const Matrix<Integer>& M) const;
    Matrix<Integer> to_sublattice_dual(const Matrix<Integer>& M) const;
    Matrix<Integer> from_sublattice_dual(const Matrix<Integer>& M) const;

    vector<Integer> to_sublattice(const vector<Integer>& V) const;
    vector<Integer> from_sublattice(const vector<Integer>& V) const;
    vector<Integer> to_sublattice_dual(const vector<Integer>& M) const;
    vector<Integer> from_sublattice_dual(const vector<Integer>& V) const;

    vector<Integer> to_sublattice_dual_no_div(const vector<Integer>& M) const;

    // and with integrated type conversion
    // Note: the "to" conversions assume that val has the same integer type as the SLR
    // whereas the "from" versions assume that ret has the same integer type as the SLR.

    // We give special attention to the "from" versions for matrices
    // because they must possibly transform big objects

    template <typename ToType, typename FromType>
    void convert_to_sublattice(ToType& ret, const FromType& val) const {
        convert(ret, to_sublattice(val));
    }

    template <typename ToType>
    void convert_to_sublattice(Matrix<ToType>& ret, const Matrix<Integer>& val) const {
        ret = Matrix<ToType>(val.nr_of_rows(), rank);
        vector<Integer> v;
        for (size_t i = 0; i < val.nr_of_rows(); ++i) {
            v = to_sublattice(val[i]);
            convert(ret[i], v);
        }
    }

    template <typename ToType, typename FromType>
    void convert_from_sublattice(ToType& ret, const FromType& val) const {
        ret = from_sublattice(convertTo<ToType>(val));
    }

    template <typename FromType>
    void convert_from_sublattice(Matrix<Integer>& ret, const Matrix<FromType>& val) const;

    void convert_from_sublattice(Matrix<Integer>& ret, const Matrix<Integer>& val) const;

    template <typename ToType, typename FromType>
    void convert_to_sublattice_dual(ToType& ret, const FromType& val) const {
        convert(ret, to_sublattice_dual(val));
    }

    template <typename ToType>
    void convert_to_sublattice_dual(Matrix<ToType>& ret, const Matrix<Integer>& val) const {
        ret = Matrix<ToType>(val.nr_of_rows(), rank);
        vector<Integer> v;
        for (size_t i = 0; i < val.nr_of_rows(); ++i) {
            v = to_sublattice_dual(val[i]);
            convert(ret[i], v);
        }
    }

    template <typename ToType, typename FromType>
    void convert_from_sublattice_dual(ToType& ret, const FromType& val) const {
        ret = from_sublattice_dual(convertTo<ToType>(val));
    }

    template <typename FromType>
    void convert_from_sublattice_dual(Matrix<Integer>& ret, const Matrix<FromType>& val) const;

    void convert_from_sublattice_dual(Matrix<Integer>& ret, const Matrix<Integer>& val) const;

    template <typename ToType, typename FromType>
    void convert_to_sublattice_dual_no_div(ToType& ret, const FromType& val) const {
        convert(ret, to_sublattice_dual_no_div(val));
    }

    template <typename ToType>
    void convert_to_sublattice_dual_no_div(Matrix<ToType>& ret, const Matrix<Integer>& val) const {
        ret = Matrix<ToType>(val.nr_of_rows(), rank);
        vector<Integer> v;
        for (size_t i = 0; i < val.nr_of_rows(); ++i) {
            v = to_sublattice_dual_no_div(val[i]);
            convert(ret[i], v);
        }
    }

    //---------------------------------------------------------------------------
    //						 Data access
    //---------------------------------------------------------------------------

    /* returns the dimension of the ambient space */
    size_t getDim() const;

    /* returns the rank of the sublattice */
    size_t getRank() const;

    inline vector<key_t> getProjectionKey() const {
        return projection_key;
    }

    Integer getAnnihilator() const;
    bool IsIdentity() const;

    const Matrix<Integer>& getEquationsMatrix() const;
    const vector<vector<Integer> >& getEquations() const;
    const Matrix<Integer>& getCongruencesMatrix() const;
    const vector<vector<Integer> >& getCongruences() const;
    mpz_class getExternalIndex() const;
    const Matrix<Integer>& getEmbeddingMatrix() const;
    const vector<vector<Integer> >& getEmbedding() const;
    const Matrix<Integer>& getProjectionMatrix() const;
    const vector<vector<Integer> >& getProjection() const;

    bool equal(const Sublattice_Representation& SLR) const;
};

//---------------------------------------------------------------------------
//                       Constructor by conversion
//---------------------------------------------------------------------------

template <typename Integer>
template <typename IntegerFC>
Sublattice_Representation<Integer>::Sublattice_Representation(const Sublattice_Representation<IntegerFC>& Original) {
    convert(A, Original.A);
    convert(B, Original.B);
    dim = Original.dim;
    rank = Original.rank;
    convert(c, Original.c);
    is_identity = Original.is_identity;
    Equations_computed = Original.Equations_computed;
    Congruences_computed = Original.Congruences_computed;
    convert(Equations, Original.Equations);
    convert(Congruences, Original.Congruences);
    external_index = Original.external_index;
    projection_key = Original.projection_key;
    B_is_projection = Original.B_is_projection;
}

//---------------------------------------------------------------------------
//                       LLL coordinates
//---------------------------------------------------------------------------

template <typename Integer, typename number>
Sublattice_Representation<Integer> LLL_coordinates(const Matrix<number>& G) {
    // direction from given coorfinates to LLL_coordinates is "to"

    Matrix<Integer> T, Tinv;
    LLL_red_transpose(G, T, Tinv);  // Tinv <<--> A, T <--> B
    /*Matrix<Integer> IntT,IntTinv;
    convert(IntT,T);
    convert(IntTinv,Tinv);*/
    // return Sublattice_Representation<Integer>(IntTinv,IntT,1);
    return Sublattice_Representation<Integer>(Tinv, T, 1);
}

vector<key_t> reverse_key(size_t n);
vector<key_t> identity_key(size_t n);

template <typename Integer, typename number>
Sublattice_Representation<Integer> LLL_coordinates_dual(const Matrix<number>& G) {
    // direction from given coorfinates to LLL_coordinates is "to"

    Matrix<Integer> T, Tinv;
    LLL_red_transpose(G, T, Tinv);  // T <---> A^tr, Tinv <--> B^tr
    /*Matrix<Integer> IntT,IntTinv;
    convert(IntT,T);
    convert(IntTinv,Tinv); // but we reverse the order of the coordinates*/
    vector<key_t> reverse = reverse_key(T.nr_of_columns());

    /* IntT=IntT.transpose();
    IntT=IntT.submatrix(reverse); // rows of A reversed
    ntTinv=IntTinv.submatrix(reverse); // after transposition below, columns are reversed*/
    T = T.transpose();
    T = T.submatrix(reverse);
    Tinv = Tinv.submatrix(reverse);  // after transposition below, columns are reversed

    // return Sublattice_Representation<Integer>(IntT,IntTinv.transpose(),1);
    return Sublattice_Representation<Integer>(T, Tinv.transpose(), 1);
}

template <typename Integer, typename number>
void LLL_coordinates_without_1st_col(Sublattice_Representation<Integer>& LLL_Coordinates,
                                     const Matrix<number> Supps,
                                     const Matrix<number> Vertices,
                                     bool verbose) {
    // used when the 1st column is the grading or the dehomogenization and should bot be changed
    // Important in project_and_lift
    // Computed SLR is returned in LLL_Coordinates

    Matrix<Integer> HelpA, HelpB;
    Integer HelpC;

    assert(Supps.nr_of_rows() > 0);
    size_t EmbDim = Supps.nr_of_columns();

    if (Vertices.nr_of_rows() == 0 || Vertices.rank() < EmbDim) {  // use Supps for LLL coordinates
        Matrix<nmz_float> SuppHelp = Supps.nmz_float_without_first_column();
        if (SuppHelp.rank() < EmbDim - 1)
            return;
        Sublattice_Representation<Integer> HelpCoord = LLL_coordinates_dual<Integer, nmz_float>(SuppHelp);
        convert(HelpA, HelpCoord.getEmbeddingMatrix());
        convert(HelpB, HelpCoord.getProjectionMatrix());
        convert(HelpC, HelpCoord.getAnnihilator());
        if (verbose)
            verboseOutput() << "LLL based on support hyperplanes" << endl;
    }
    else {  // use Vertices for LLL coordinates
        Matrix<nmz_float> VertHelp = Vertices.nmz_float_without_first_column();
        if (VertHelp.rank() < EmbDim - 1)
            return;
        Sublattice_Representation<Integer> HelpCoord = LLL_coordinates<Integer, nmz_float>(VertHelp);
        convert(HelpA, HelpCoord.getEmbeddingMatrix());
        convert(HelpB, HelpCoord.getProjectionMatrix());
        convert(HelpC, HelpCoord.getAnnihilator());
        if (verbose)
            verboseOutput() << "LLL based on vertices" << endl;
    }

    Matrix<Integer> A(EmbDim), B(EmbDim);

    // Insert into EmbDim-1 last coordinates of LLL_Coord
    for (size_t i = 0; i < EmbDim - 1; ++i)
        for (size_t j = 0; j < EmbDim - 1; ++j) {
            A[i + 1][j + 1] = HelpA[i][j];
            B[i + 1][j + 1] = HelpB[i][j];
        }

    LLL_Coordinates = Sublattice_Representation<Integer>(A, B, HelpC);
}

/*
template<typename Integer, typename number>
Sublattice_Representation<Integer> LLL_coordinates_dual(const Matrix<number>& G){
// direction from given coorfinates to LLL_coordinates is "to"

    Matrix<number> T,Tinv;
    G.LLL_red_transpose(T,Tinv);  // T <---> A^tr, Tinv <--> B^tr
    Matrix<Integer> IntT,IntTinv;
    convert(IntT,T);
    convert(IntTinv,Tinv);
    return Sublattice_Representation<Integer>(IntT.transpose(),IntTinv.transpose(),1);
}
*/

}  // namespace libnormaliz

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
