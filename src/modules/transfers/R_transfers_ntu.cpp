/*################################################################################
  ##
  ##   Copyright (C) 2015 - 2017 the TraME Team:
  ##      Alfred Galichon
  ##      Keith O'Hara
  ##
  ##   This file is part of TraME.
  ##
  ##   TraME is free software: you can redistribute it and/or modify
  ##   it under the terms of the GNU General Public License as published by
  ##   the Free Software Foundation, either version 2 of the License, or
  ##   (at your option) any later version.
  ##
  ##   TraME is distributed in the hope that it will be useful,
  ##   but WITHOUT ANY WARRANTY; without even the implied warranty of
  ##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ##   GNU General Public License for more details.
  ##
  ##   You should have received a copy of the GNU General Public License
  ##   along with TraME. If not, see <http://www.gnu.org/licenses/>.
  ##
  ################################################################################*/

/*
 * NTU transfers class module
 *
 * Keith O'Hara
 * 11/08/2016
 *
 * This version:
 * 09/13/2017
 */

 #include "ancillary/ancillary.hpp"
 #include "mmfs/mmfs.hpp"
 #include "transfers/transfers.hpp"

 #include "R_transfers.hpp"

RCPP_MODULE(transfers_ntu_module)
{
    using namespace Rcpp ;

    // function overloading requires some trickery
    void (transfers_ntu_R::*build_1)(const arma::mat&, const arma::mat&) = &transfers_ntu_R::build_R ;
    void (transfers_ntu_R::*build_2)(const arma::mat&, const arma::mat&, bool) = &transfers_ntu_R::build_R ;

    SEXP (transfers_ntu_R::*Psi_1)(const arma::mat&, const arma::mat&) = &transfers_ntu_R::Psi_R ;
    SEXP (transfers_ntu_R::*Psi_2)(const arma::mat&, const arma::mat&, Rcpp::IntegerVector, Rcpp::IntegerVector) = &transfers_ntu_R::Psi_R ;

    SEXP (transfers_ntu_R::*du_Psi_1)(const arma::mat&, const arma::mat&) = &transfers_ntu_R::du_Psi_R ;
    SEXP (transfers_ntu_R::*du_Psi_2)(const arma::mat&, const arma::mat&, Rcpp::IntegerVector, Rcpp::IntegerVector) = &transfers_ntu_R::du_Psi_R ;

    SEXP (transfers_ntu_R::*Ucal_1)(const arma::mat&) = &transfers_ntu_R::Ucal_R ;
    SEXP (transfers_ntu_R::*Ucal_2)(const arma::mat&, Rcpp::IntegerVector, Rcpp::IntegerVector) = &transfers_ntu_R::Ucal_R ;

    SEXP (transfers_ntu_R::*Vcal_1)(const arma::mat&) = &transfers_ntu_R::Vcal_R ;
    SEXP (transfers_ntu_R::*Vcal_2)(const arma::mat&, Rcpp::IntegerVector, Rcpp::IntegerVector) = &transfers_ntu_R::Vcal_R ;

    // now we can declare the class
    class_<trame::transfers::ntu>( "transfers_ntu_cpp" )
        .default_constructor()

        // basic objects
        .field( "nbX", &trame::transfers::ntu::nbX )
        .field( "nbY", &trame::transfers::ntu::nbY )
        .field( "dim_params", &trame::transfers::ntu::dim_params )
        
        .field( "alpha", &trame::transfers::ntu::alpha )
        .field( "gamma", &trame::transfers::ntu::gamma )

        // read only objects
        .field_readonly( "transfers_type", &trame::transfers::ntu::transfers_type )

        // member functions
    ;

    class_<transfers_ntu_R>( "transfers_ntu" )
        .derives<trame::transfers::ntu>( "transfers_ntu_cpp" )
        .default_constructor()

        .method( "build", build_1 )
        .method( "build", build_2 )

        .method( "trans", &transfers_ntu_R::trans_R )

        .method( "Psi", Psi_1 )
        .method( "Psi", Psi_2 )
        .method( "du_Psi", du_Psi_1 )
        .method( "du_Psi", du_Psi_2 )
        .method( "Ucal", Ucal_1 )
        .method( "Ucal", Ucal_2 )
        .method( "Vcal", Vcal_1 )
        .method( "Vcal", Vcal_2 )
    ;
}

// wrapper functions to catch errors and handle memory pointers
void transfers_ntu_R::build_R(const arma::mat& alpha_inp, const arma::mat& gamma_inp)
{
    try {
        this->build(alpha_inp,gamma_inp,false);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
}

void transfers_ntu_R::build_R(const arma::mat& alpha_inp, const arma::mat& gamma_inp, bool need_norm_inp)
{
    try {
        this->build(alpha_inp,gamma_inp,need_norm_inp);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
}

void transfers_ntu_R::trans_R()
{
    try {
        this->trans();
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
}

SEXP transfers_ntu_R::Psi_R(const arma::mat& U, const arma::mat& V)
{
    try {
        arma::mat psi_out = this->Psi(U,V);
        //
        return Rcpp::wrap(psi_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}

SEXP transfers_ntu_R::Psi_R(const arma::mat& U, const arma::mat& V, Rcpp::IntegerVector x_ind, Rcpp::IntegerVector y_ind)
{
    try {
        int x_ind_size = x_ind.size();
        int y_ind_size = y_ind.size();

        arma::mat psi_out;
        //
        // default case to mirror NULL
        if (x_ind_size == 0 && y_ind_size == 0) {
            psi_out = this->Psi(U,V);
        }
        //
        // correct for zero indexing vs R indexing
        arma::uvec x_ind_uvec, y_ind_uvec;

        if (x_ind_size != 0) {
            x_ind_uvec = Rcpp::as<arma::uvec>(x_ind) - 1;
        }
        if (y_ind_size != 0) {
            y_ind_uvec = Rcpp::as<arma::uvec>(y_ind) - 1;
        }
        //
        if (x_ind_size != 0 && y_ind_size == 0) {
            psi_out = this->Psi(U,V,&x_ind_uvec,NULL);
        } else if (x_ind_size == 0 && y_ind_size != 0) {
            psi_out = this->Psi(U,V,NULL,&y_ind_uvec);
        } else {
            psi_out = this->Psi(U,V,&x_ind_uvec,&y_ind_uvec);
        }
        //
        return Rcpp::wrap(psi_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}

SEXP transfers_ntu_R::du_Psi_R(const arma::mat& U, const arma::mat& V)
{
    try {
        arma::mat du_psi_out = this->du_Psi(U,V);
        //
        return Rcpp::wrap(du_psi_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}

SEXP transfers_ntu_R::du_Psi_R(const arma::mat& U, const arma::mat& V, Rcpp::IntegerVector x_ind, Rcpp::IntegerVector y_ind)
{
    try {
        int x_ind_size = x_ind.size();
        int y_ind_size = y_ind.size();

        arma::mat du_psi_out;
        //
        // default case to mirror NULL
        if (x_ind_size == 0 && y_ind_size == 0) {
            du_psi_out = this->du_Psi(U,V);
        }
        //
        // correct for zero indexing vs R indexing
        arma::uvec x_ind_uvec, y_ind_uvec;

        if (x_ind_size != 0) {
            x_ind_uvec = Rcpp::as<arma::uvec>(x_ind) - 1;
        }
        if (y_ind_size != 0) {
            y_ind_uvec = Rcpp::as<arma::uvec>(y_ind) - 1;
        }
        //
        if (x_ind_size != 0 && y_ind_size == 0) {
            du_psi_out = this->du_Psi(U,V,&x_ind_uvec,NULL);
        } else if (x_ind_size == 0 && y_ind_size != 0) {
            du_psi_out = this->du_Psi(U,V,NULL,&y_ind_uvec);
        } else {
            du_psi_out = this->du_Psi(U,V,&x_ind_uvec,&y_ind_uvec);
        }
        //
        return Rcpp::wrap(du_psi_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}

SEXP transfers_ntu_R::Ucal_R(const arma::mat& vs)
{
    try {
        arma::mat ucal_out = this->Ucal(vs);
        //
        return Rcpp::wrap(ucal_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}

SEXP transfers_ntu_R::Ucal_R(const arma::mat& vs, Rcpp::IntegerVector x_ind, Rcpp::IntegerVector y_ind)
{
    try {
        int x_ind_size = x_ind.size();
        int y_ind_size = y_ind.size();

        arma::mat ucal_out;
        //
        // default case to mirror NULL
        if (x_ind_size == 0 && y_ind_size == 0) {
            ucal_out = this->Ucal(vs);
        }
        //
        // correct for zero indexing vs R indexing
        arma::uvec x_ind_uvec, y_ind_uvec;

        if (x_ind_size != 0) {
            x_ind_uvec = Rcpp::as<arma::uvec>(x_ind) - 1;
        }
        if (y_ind_size != 0) {
            y_ind_uvec = Rcpp::as<arma::uvec>(y_ind) - 1;
        }
        //
        if (x_ind_size != 0 && y_ind_size == 0) {
            ucal_out = this->Ucal(vs,&x_ind_uvec,NULL);
        } else if (x_ind_size == 0 && y_ind_size != 0) {
            ucal_out = this->Ucal(vs,NULL,&y_ind_uvec);
        } else {
            ucal_out = this->Ucal(vs,&x_ind_uvec,&y_ind_uvec);
        }
        //
        return Rcpp::wrap(ucal_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}

SEXP transfers_ntu_R::Vcal_R(const arma::mat& us)
{
    try {
        arma::mat vcal_out = this->Vcal(us);
        //
        return Rcpp::wrap(vcal_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}

SEXP transfers_ntu_R::Vcal_R(const arma::mat& us, Rcpp::IntegerVector x_ind, Rcpp::IntegerVector y_ind)
{
    try {
        int x_ind_size = x_ind.size();
        int y_ind_size = y_ind.size();

        arma::mat vcal_out;
        //
        // default case to mirror NULL
        if (x_ind_size == 0 && y_ind_size == 0) {
            vcal_out = this->Vcal(us);
        }
        //
        // correct for zero indexing vs R indexing
        arma::uvec x_ind_uvec, y_ind_uvec;

        if (x_ind_size != 0) {
            x_ind_uvec = Rcpp::as<arma::uvec>(x_ind) - 1;
        }
        if (y_ind_size != 0) {
            y_ind_uvec = Rcpp::as<arma::uvec>(y_ind) - 1;
        }
        //
        if (x_ind_size != 0 && y_ind_size == 0) {
            vcal_out = this->Vcal(us,&x_ind_uvec,NULL);
        } else if (x_ind_size == 0 && y_ind_size != 0) {
            vcal_out = this->Vcal(us,NULL,&y_ind_uvec);
        } else {
            vcal_out = this->Vcal(us,&x_ind_uvec,&y_ind_uvec);
        }
        //
        return Rcpp::wrap(vcal_out);
    } catch( std::exception &ex ) {
        forward_exception_to_r( ex );
    } catch(...) {
        ::Rf_error( "trame: C++ exception (unknown reason)" );
    }
    return R_NilValue;
}
