/*################################################################################
  ##
  ##   Copyright (C) 2015 - 2016 the TraME Team:
  ##      Alfred Galichon
  ##      Keith O'Hara
  ##      Simon Weber
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
 * transfers class
 *
 * Keith O'Hara
 * 08/16/2016
 */

#include "trame.hpp"

void trame::transfers::build_ETU(arma::mat alpha_ETU, arma::mat gamma_ETU, arma::mat tau_ETU)
{
    alpha = alpha_ETU;
    gamma = gamma_ETU;
    tau   = tau_ETU;

    nbX = alpha_ETU.n_rows;
    nbY = alpha_ETU.n_cols;
    nbParams = 3*nbX*nbY;

    aux_exp_alphaovertau = arma::exp(- alpha / tau);
    aux_exp_gammaovertau = arma::exp(- gamma / tau);

    ETU = true;
}

void trame::transfers::build_LTU(arma::mat lambda_LTU, arma::mat phi_LTU)
{
    lambda = lambda_LTU;
    phi    = phi_LTU;

    nbX = lambda_LTU.n_rows;
    nbY = lambda_LTU.n_cols;
    nbParams = 2*nbX*nbY;

    aux_zeta = 1 - lambda;

    LTU = true;
}

void trame::transfers::build_NTU(arma::mat alpha_NTU, arma::mat gamma_NTU)
{
    alpha = alpha_NTU;
    gamma = gamma_NTU;

    nbX = alpha_NTU.n_rows;
    nbY = alpha_NTU.n_cols;
    nbParams = 2*nbX*nbY;

    NTU = true;
}

void trame::transfers::build_TU(arma::mat phi_TU)
{
    phi = phi_TU;

    nbX = phi_TU.n_rows;
    nbY = phi_TU.n_cols;
    nbParams = nbX*nbY;

    TU = true;
}

void trame::transfers::trans()
{
    int nbX_temp;
    //
    if (ETU) {
        nbX_temp = nbX;
        nbX = nbY;
        nbY = nbX_temp;

        arma::mat alpha_temp = alpha;

        alpha = gamma.t();
        gamma = alpha_temp.t();
        tau   = tau.t();

        arma::mat aux_exp_alphaovertau_temp = aux_exp_alphaovertau;

        aux_exp_alphaovertau = aux_exp_gammaovertau.t();
        aux_exp_gammaovertau = aux_exp_alphaovertau_temp.t();
    }

    if (LTU) {
        nbX_temp = nbX;
        nbX = nbY;
        nbY = nbX_temp;

        arma::mat lambda_temp = lambda;

        lambda   = aux_zeta.t();
        phi      = phi.t();
        aux_zeta = lambda_temp.t();
    }

    if (NTU) {
        nbX_temp = nbX;
        nbX = nbY;
        nbY = nbX_temp;

        arma::mat alpha_temp = alpha;

        alpha = gamma.t();
        gamma = alpha_temp.t();
    }

    if (TU) {
        nbX_temp = nbX;
        nbX = nbY;
        nbY = nbX_temp;

        phi = phi.t();
    }
}

arma::mat trame::transfers::Psi(arma::mat U, arma::mat V)
{
    arma::mat ret(nbX,nbY);
    //
    if (ETU) {
        ret =  tau % arma::log(0.5 * (arma::exp(U/tau) % aux_exp_alphaovertau + arma::exp(V/tau) % aux_exp_gammaovertau));
        goto finished;
    }

    if (LTU) {
        ret = lambda % U + aux_zeta % V - phi;
        goto finished;
    }

    if (NTU) {
        ret = arma::max(U - alpha, V - gamma);
        goto finished;
    }

    if (TU) {
        ret = (U + V - phi) / 2;
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::Psi(double U, arma::mat V, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret(nbX,nbY);
    //
    if (ETU) {
        ret =  tau % arma::log(0.5 * (arma::exp(U/tau(xs,ys)) % aux_exp_alphaovertau(xs,ys) + arma::exp(V/tau(xs,ys)) % aux_exp_gammaovertau(xs,ys)));
        goto finished;
    }

    if (LTU) {
        ret = lambda(xs,ys) * U + aux_zeta(xs,ys) % V - phi(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret = arma::max(U - alpha(xs,ys), V - gamma(xs,ys));
        goto finished;
    }

    if (TU) {
        ret = (U + V - phi(xs,ys)) / 2;
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::Psi(arma::mat U, double V, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret(nbX,nbY);
    //
    if (ETU) {
        ret =  tau % arma::log(0.5 * (arma::exp(U/tau(xs,ys)) % aux_exp_alphaovertau(xs,ys) + arma::exp(V/tau(xs,ys)) % aux_exp_gammaovertau(xs,ys)));
        goto finished;
    }

    if (LTU) {
        ret = lambda(xs,ys) % U + aux_zeta(xs,ys) * V - phi(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret = arma::max(U - alpha(xs,ys), V - gamma(xs,ys));
        goto finished;
    }

    if (TU) {
        ret = (U + V - phi(xs,ys)) / 2;
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::Psi(arma::mat U, arma::mat V, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret(nbX,nbY);
    //
    if (ETU) {
        ret =  tau(xs,ys) % arma::log(0.5 * (arma::exp(U/tau(xs,ys)) % aux_exp_alphaovertau(xs,ys) + arma::exp(V/tau(xs,ys)) % aux_exp_gammaovertau(xs,ys)));
        goto finished;
    }

    if (LTU) {
        ret = lambda(xs,ys) % U + aux_zeta(xs,ys) % V - phi(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret = arma::max(U - alpha(xs,ys), V - gamma(xs,ys));
        goto finished;
    }

    if (TU) {
        ret = (U + V - phi(xs,ys)) / 2;
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::du_Psi(arma::mat U, arma::mat V)
{
    arma::mat ret(nbX,nbY);
    //
    if (ETU) {
        ret =  1 / (1 + arma::exp((V - U + alpha - gamma)/tau));
        goto finished;
    }

    if (LTU) {
        ret = lambda;
        goto finished;
    }

    if (NTU) {
        ret.elem( arma::find(U - alpha >= V - gamma) ).ones();
        goto finished;
    }

    if (TU) {
        ret.fill(0.5);
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::du_Psi(double U, arma::mat V, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret(xs.n_elem,ys.n_elem);
    //
    if (ETU) {
        ret =  1 / (1 + arma::exp((V - U + alpha(xs,ys) - gamma(xs,ys))/tau(xs,ys)));
        goto finished;
    }

    if (LTU) {
        ret = lambda(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret.elem( arma::find(U - alpha(xs,ys) >= V - gamma(xs,ys)) ).ones();
        goto finished;
    }

    if (TU) {
        ret.fill(0.5);
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::du_Psi(arma::mat U, double V, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret(xs.n_elem,ys.n_elem);
    //
    if (ETU) {
        ret =  1 / (1 + arma::exp((V - U + alpha(xs,ys) - gamma(xs,ys))/tau(xs,ys)));
        goto finished;
    }

    if (LTU) {
        ret = lambda(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret.elem( arma::find(U - alpha(xs,ys) >= V - gamma(xs,ys)) ).ones();
        goto finished;
    }

    if (TU) {
        ret.fill(0.5);
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::du_Psi(arma::mat U, arma::mat V, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret(xs.n_elem,ys.n_elem);
    //
    if (ETU) {
        ret =  1 / (1 + arma::exp((V - U + alpha(xs,ys) - gamma(xs,ys))/tau(xs,ys)));
        goto finished;
    }

    if (LTU) {
        ret = lambda(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret.elem( arma::find(U - alpha(xs,ys) >= V - gamma(xs,ys)) ).ones();
        goto finished;
    }

    if (TU) {
        ret.fill(0.5);
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::dtheta_Psi(arma::mat U, arma::mat V, arma::mat* dtheta)
{
    arma::mat ret(nbX,nbY);
    //
    if (ETU) {
        arma::mat dupsi_mat = du_Psi(U,V);
        arma::vec dupsi = arma::vectorise(dupsi_mat);

        if (!dtheta) {
            arma::mat term_1, term_2;
            term_1 = (U - alpha) % dupsi_mat;
            term_2 = (V - gamma) % (1 - dupsi_mat);

            arma::mat dsigmapsi_mat = (Psi(U,V) - term_1 - term_2)/tau;
            arma::vec dsigmapsi = arma::vectorise(dsigmapsi_mat);
            //
            ret = arma::join_rows(arma::diagmat(-dupsi),arma::join_rows(arma::diagmat(dupsi-1),arma::diagmat(dsigmapsi)));
            goto finished;
        } else {
            arma::mat dtheta_1 = dtheta->rows(0,nbX*nbY-1);
            arma::mat dtheta_2 = dtheta->rows(nbX*nbY,2*nbX*nbY-1);
            arma::mat dtheta_3 = dtheta->rows(2*nbX*nbY,3*nbX*nbY-1);

            arma::vec dsigmapsidtheta = arma::zeros(dtheta_3.n_rows,1);
            double min_check = arma::as_scalar(arma::min(arma::min(dtheta_3)));

            if (min_check!=0) {
                arma::mat term_1, term_2;
                term_1 = (U - alpha) % dupsi_mat;
                term_2 = (V - gamma) % (1 - dupsi_mat);

                arma::mat dsigmapsi_mat = (Psi(U,V) - term_1 - term_2)/tau;

                dsigmapsidtheta = dtheta_3 % arma::vectorise(dsigmapsi_mat);
            }
            //
            ret = arma::vectorise(-dupsi % dtheta_1 - (1-dupsi) % dtheta_2 + dsigmapsidtheta);
            goto finished;
        }
    }

    if (LTU) {
        arma::vec U_minus_V = arma::vectorise(U-V);

        if (!dtheta) {
            ret = arma::join_rows(arma::diagmat(U_minus_V),-arma::eye(nbX*nbY,nbX*nbY));
            goto finished;
        } else {
            arma::mat dtheta_1 = dtheta->rows(0,nbX*nbY-1);
            arma::mat dtheta_2 = dtheta->rows(nbX*nbY,2*nbX*nbY-1);
            //
            ret = arma::vectorise(U_minus_V % dtheta_1 - dtheta_2);
            goto finished;
        }
    }

    if (NTU) {
        arma::vec dupsi = arma::vectorise(du_Psi(U,V));

        if (!dtheta) {
            ret = - arma::join_rows(arma::diagmat(dupsi),arma::diagmat(1 - dupsi));
            goto finished;
        } else {
            arma::mat dtheta_1 = dtheta->rows(0,nbX*nbY-1);
            arma::mat dtheta_2 = dtheta->rows(nbX*nbY,2*nbX*nbY-1);

            ret = - arma::vectorise(dupsi % dtheta_1 + (1 - dupsi) % dtheta_2);
            goto finished;
        }
    }

    if (TU) {
        if (!dtheta) {
            ret = - 0.5*arma::eye(nbX*nbY,nbX*nbY);
            goto finished;
        } else {
            ret = - (*dtheta)/2;
            goto finished;
        }
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::Ucal(arma::mat vs, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret;
    //
    if (ETU) {
        arma::mat term_1 = vs - gamma(xs,ys);
        arma::mat term_log = 2 - arma::exp(term_1/tau(xs,ys));

        ret = alpha(xs,ys) + tau(xs,ys) % arma::log(term_log);
        goto finished;
    }

    if (LTU) {
        arma::mat term_1 = phi(xs,ys) - aux_zeta(xs,ys) % vs;
        arma::mat term_2 = lambda(xs,ys);

        ret = term_1 / term_2;
        goto finished;
    }
    /* no Ucal for NTU?
    if (NTU) {

    }
    */
    if (TU) {
        ret = phi(xs,ys) - vs;
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::Vcal(arma::mat us, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret;
    //
    if (ETU) {
        arma::mat term_1 = us - alpha(xs,ys);
        arma::mat term_log = 2 - arma::exp(term_1/tau(xs,ys));

        ret = gamma(xs,ys) + tau(xs,ys) % arma::log(term_log);
        goto finished;
    }

    if (LTU) {
        arma::mat term_1 = phi(xs,ys) - lambda(xs,ys) % us;
        arma::mat term_2 = aux_zeta(xs,ys);

        ret = term_1 / term_2;
        goto finished;
    }
    /* no Ucal for NTU?
    if (NTU) {

    }
    */
    if (TU) {
        ret = phi(xs,ys) - us;
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::UW(arma::mat Ws, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret = - Psi(0.0,-Ws,xs,ys);

    return ret;
}

arma::mat trame::transfers::VW(arma::mat Ws, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret = - Psi(Ws,0.0,xs,ys);

    return ret;
}

arma::mat trame::transfers::du_UW(arma::mat Ws, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret = 1 - du_Psi(0.0,-Ws,xs,ys);

    return ret;
}

arma::mat trame::transfers::du_VW(arma::mat Ws, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret = - du_Psi(Ws,0.0,xs,ys);

    return ret;
}

arma::mat trame::transfers::WU(arma::mat Us, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret;
    //
    if (ETU) {
        arma::mat term_1 = 2 * arma::exp( (gamma(xs,ys) - Us)/tau(xs,ys) );
        arma::mat term_2 = arma::exp( (gamma(xs,ys) - alpha(xs,ys))/tau(xs,ys) );
        arma::mat term_log = term_1 - term_2;

        ret = - tau(xs,ys) % arma::log(term_log);
        goto finished;
    }

    if (LTU) {
        ret = (Us - phi(xs,ys)) / aux_zeta(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret = Us - alpha(xs,ys);
        goto finished;
    }

    if (TU) {
        ret = 2*Us - phi(xs,ys);
        goto finished;
    }
    //
finished:
    return ret;
}

arma::mat trame::transfers::WV(arma::mat Vs, arma::uvec xs, arma::uvec ys)
{
    arma::mat ret;
    //
    if (ETU) {
        arma::mat term_1 = 2 * arma::exp( (alpha(xs,ys) - Vs)/tau(xs,ys) );
        arma::mat term_2 = arma::exp( (alpha(xs,ys) - gamma(xs,ys))/tau(xs,ys) );
        arma::mat term_log = term_1 - term_2;

        ret = - tau(xs,ys) % arma::log(term_log);
        goto finished;
    }

    if (LTU) {
        ret = (phi(xs,ys) - Vs) / lambda(xs,ys);
        goto finished;
    }

    if (NTU) {
        ret = gamma(xs,ys) - Vs;
        goto finished;
    }

    if (TU) {
        ret = phi(xs,ys) - 2*Vs;
        goto finished;
    }
    //
finished:
    return ret;
}
