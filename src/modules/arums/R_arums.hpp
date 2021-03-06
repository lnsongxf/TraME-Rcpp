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
 * Derived classes to provide wrappers to the TraME library
 */

#ifndef _R_arums_HPP
#define _R_arums_HPP

class empirical_R : public trame::arums::empirical
{
    public:
        SEXP build_R(int nbX_inp, int nbY_inp);
        SEXP G_R(const arma::vec& n);
        SEXP G_R(const arma::vec& n, const arma::mat& U_inp);
        SEXP Gx_R(const arma::mat& U_x_inp, int x);
        SEXP Gstar_R(const arma::vec& n);
        SEXP Gstar_R(const arma::vec& n, const arma::mat& mu_inp);
        SEXP Gstarx_R(const arma::mat& mu_x_inp, int x);
        SEXP Gbar_R(const arma::mat& U_bar, const arma::mat& mu_bar, const arma::vec& n);
};

class logit_R : public trame::arums::logit
{
    public:
        SEXP G_R(const arma::vec& n);
        SEXP G_R(const arma::vec& n, const arma::mat& U_inp);
        SEXP Gstar_R(const arma::vec& n);
        SEXP Gstar_R(const arma::vec& n, const arma::mat& mu_inp);
        SEXP Gbar_R(const arma::mat& U_bar, const arma::mat& mu_bar, const arma::vec& n);

        empirical_R simul_R(int n_draws);
};

class none_R : public trame::arums::none
{
    public:
        SEXP G_R(const arma::vec& n);
        SEXP G_R(const arma::vec& n, const arma::mat& U_inp);
        SEXP Gx_R(const arma::mat& U_x_inp, int x);
        SEXP Gstar_R(const arma::vec& n);
        SEXP Gstar_R(const arma::vec& n, const arma::mat& mu_inp);
        SEXP Gstarx_R(const arma::mat& mu_x_inp, int x);
        SEXP Gbar_R(const arma::mat& U_bar, const arma::mat& mu_bar, const arma::vec& n);

        empirical_R simul_R(int n_draws);
};

class probit_R : public trame::arums::probit
{
    public:
        empirical_R simul_R(int n_draws);
};

class rsc_R : public trame::arums::rsc
{
    public:
        SEXP G_R(const arma::vec& n);
        SEXP G_R(const arma::vec& n, const arma::mat& U_inp);
        SEXP Gx_R(const arma::mat& U_x_inp, int x);
        SEXP Gstar_R(const arma::vec& n);
        SEXP Gstar_R(const arma::vec& n, const arma::mat& mu_inp);
        SEXP Gstarx_R(const arma::mat& mu_x_inp, int x);
        SEXP Gbar_R(const arma::mat& U_bar, const arma::mat& mu_bar, const arma::vec& n);

        empirical_R simul_R(int n_draws);
};

class rusc_R : public trame::arums::rusc
{
    public:
        SEXP G_R(const arma::vec& n);
        SEXP G_R(const arma::vec& n, const arma::mat& U_inp);
        SEXP Gx_R(const arma::mat& U_x_inp, int x);
        SEXP Gstar_R(const arma::vec& n);
        SEXP Gstar_R(const arma::vec& n, const arma::mat& mu_inp);
        SEXP Gstarx_R(const arma::mat& mu_x_inp, int x);
        SEXP Gbar_R(const arma::mat& U_bar, const arma::mat& mu_bar, const arma::vec& n);

        empirical_R simul_R(int n_draws);
};

#endif
