#ifndef BOOST_GEOMETRY_PROJECTIONS_BONNE_HPP
#define BOOST_GEOMETRY_PROJECTIONS_BONNE_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2017.
// Modifications copyright (c) 2017, Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/geometry/util/math.hpp>
#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/srs/projections/impl/base_static.hpp>
#include <boost/geometry/srs/projections/impl/base_dynamic.hpp>
#include <boost/geometry/srs/projections/impl/projects.hpp>
#include <boost/geometry/srs/projections/impl/factory_entry.hpp>
#include <boost/geometry/srs/projections/impl/pj_mlfn.hpp>

namespace boost { namespace geometry
{

namespace srs { namespace par4
{
    struct bonne {};

}} //namespace srs::par4

namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace bonne
    {

            static const double EPS10 = 1e-10;

            template <typename T>
            struct par_bonne
            {
                T phi1;
                T cphi1;
                T am1;
                T m1;
                T en[EN_SIZE];
            };

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_bonne_ellipsoid : public base_t_fi<base_bonne_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;

                par_bonne<CalculationType> m_proj_parm;

                inline base_bonne_ellipsoid(const Parameters& par)
                    : base_t_fi<base_bonne_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(e_forward)  ellipsoid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    CalculationType rh, E, c;

                    rh = this->m_proj_parm.am1 + this->m_proj_parm.m1 - pj_mlfn(lp_lat, E = sin(lp_lat), c = cos(lp_lat), this->m_proj_parm.en);
                    E = c * lp_lon / (rh * sqrt(1. - this->m_par.es * E * E));
                    xy_x = rh * sin(E);
                    xy_y = this->m_proj_parm.am1 - rh * cos(E);
                }

                // INVERSE(e_inverse)  ellipsoid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    static const CalculationType HALFPI = detail::HALFPI<CalculationType>();

                    CalculationType s, rh;

                    rh = boost::math::hypot(xy_x, xy_y = this->m_proj_parm.am1 - xy_y);
                    lp_lat = pj_inv_mlfn(this->m_proj_parm.am1 + this->m_proj_parm.m1 - rh, this->m_par.es, this->m_proj_parm.en);
                    if ((s = fabs(lp_lat)) < HALFPI) {
                        s = sin(lp_lat);
                        lp_lon = rh * atan2(xy_x, xy_y) *
                           sqrt(1. - this->m_par.es * s * s) / cos(lp_lat);
                    } else if (fabs(s - HALFPI) <= EPS10)
                        lp_lon = 0.;
                    else
                        BOOST_THROW_EXCEPTION( projection_exception(-20) );
                }

                static inline std::string get_name()
                {
                    return "bonne_ellipsoid";
                }

            };

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_bonne_spheroid : public base_t_fi<base_bonne_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;

                par_bonne<CalculationType> m_proj_parm;

                inline base_bonne_spheroid(const Parameters& par)
                    : base_t_fi<base_bonne_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(s_forward)  spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    CalculationType E, rh;

                    rh = this->m_proj_parm.cphi1 + this->m_proj_parm.phi1 - lp_lat;
                    if (fabs(rh) > EPS10) {
                        xy_x = rh * sin(E = lp_lon * cos(lp_lat) / rh);
                        xy_y = this->m_proj_parm.cphi1 - rh * cos(E);
                    } else
                        xy_x = xy_y = 0.;
                }

                // INVERSE(s_inverse)  spheroid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    static const CalculationType HALFPI = detail::HALFPI<CalculationType>();

                    CalculationType rh;

                    rh = boost::math::hypot(xy_x, xy_y = this->m_proj_parm.cphi1 - xy_y);
                    lp_lat = this->m_proj_parm.cphi1 + this->m_proj_parm.phi1 - rh;
                    if (fabs(lp_lat) > HALFPI)
                        BOOST_THROW_EXCEPTION( projection_exception(-20) );
                    if (fabs(fabs(lp_lat) - HALFPI) <= EPS10)
                        lp_lon = 0.;
                    else
                        lp_lon = rh * atan2(xy_x, xy_y) / cos(lp_lat);
                }

                static inline std::string get_name()
                {
                    return "bonne_spheroid";
                }

            };

            // Bonne (Werner lat_1=90)
            template <typename Parameters, typename T>
            inline void setup_bonne(Parameters& par, par_bonne<T>& proj_parm)
            {
                static const T HALFPI = detail::HALFPI<T>();

                T c;

                proj_parm.phi1 = pj_param(par.params, "rlat_1").f;
                if (fabs(proj_parm.phi1) < EPS10)
                    BOOST_THROW_EXCEPTION( projection_exception(-23) );
                if (par.es) {
                    pj_enfn(par.es, proj_parm.en);
                    proj_parm.m1 = pj_mlfn(proj_parm.phi1, proj_parm.am1 = sin(proj_parm.phi1),
                        c = cos(proj_parm.phi1), proj_parm.en);
                    proj_parm.am1 = c / (sqrt(1. - par.es * proj_parm.am1 * proj_parm.am1) * proj_parm.am1);
                } else {
                    if (fabs(proj_parm.phi1) + EPS10 >= HALFPI)
                        proj_parm.cphi1 = 0.;
                    else
                        proj_parm.cphi1 = 1. / tan(proj_parm.phi1);
                }
            }

    }} // namespace detail::bonne
    #endif // doxygen

    /*!
        \brief Bonne (Werner lat_1=90) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Conic
         - Spheroid
         - Ellipsoid
        \par Projection parameters
         - lat_1: Latitude of first standard parallel (degrees)
        \par Example
        \image html ex_bonne.gif
    */
    template <typename CalculationType, typename Parameters>
    struct bonne_ellipsoid : public detail::bonne::base_bonne_ellipsoid<CalculationType, Parameters>
    {
        inline bonne_ellipsoid(const Parameters& par) : detail::bonne::base_bonne_ellipsoid<CalculationType, Parameters>(par)
        {
            detail::bonne::setup_bonne(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Bonne (Werner lat_1=90) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Conic
         - Spheroid
         - Ellipsoid
        \par Projection parameters
         - lat_1: Latitude of first standard parallel (degrees)
        \par Example
        \image html ex_bonne.gif
    */
    template <typename CalculationType, typename Parameters>
    struct bonne_spheroid : public detail::bonne::base_bonne_spheroid<CalculationType, Parameters>
    {
        inline bonne_spheroid(const Parameters& par) : detail::bonne::base_bonne_spheroid<CalculationType, Parameters>(par)
        {
            detail::bonne::setup_bonne(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(srs::par4::bonne, bonne_spheroid, bonne_ellipsoid)

        // Factory entry(s)
        template <typename CalculationType, typename Parameters>
        class bonne_entry : public detail::factory_entry<CalculationType, Parameters>
        {
            public :
                virtual base_v<CalculationType, Parameters>* create_new(const Parameters& par) const
                {
                    if (par.es)
                        return new base_v_fi<bonne_ellipsoid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                    else
                        return new base_v_fi<bonne_spheroid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                }
        };

        template <typename CalculationType, typename Parameters>
        inline void bonne_init(detail::base_factory<CalculationType, Parameters>& factory)
        {
            factory.add_to_factory("bonne", new bonne_entry<CalculationType, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

} // namespace projections

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_PROJECTIONS_BONNE_HPP

