/*********************************************************************
Author: Soonho Kong <soonhok@cs.cmu.edu>
        Sicun Gao <sicung@cs.cmu.edu>
        Edmund Clarke <emc@cs.cmu.edu>

dReal -- Copyright (C) 2013 - 2015, Soonho Kong, Sicun Gao, and Edmund Clarke

dReal is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

dReal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dReal. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#pragma once
#include <unordered_map>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <memory>
#include <utility>
#include "opensmt/egraph/Enode.h"
#include "util/box.h"
#include "json/json.hpp"
#include "capd/capdlib.h"
#include "contractor/contractor_basic.h"

namespace dreal {
class contractor_capd_simple : public contractor_cell {
private:
    bool const m_forward;
    ode_constraint const * const m_ctr;

public:
    contractor_capd_simple(box const & box, ode_constraint const * const ctr, bool const forward);
    void prune(box & b, SMTConfig & config) const;
    std::ostream & display(std::ostream & out) const;
};

class contractor_capd_full : public contractor_cell {
private:
    bool const m_forward;
    ode_constraint const * const m_ctr;
    unsigned const m_taylor_order;
    unsigned const m_grid_size;
    double const m_timeout;  // unit: msec

    std::unique_ptr<capd::IMap> m_vectorField;
    std::unique_ptr<capd::IOdeSolver> m_solver;
    std::unique_ptr<capd::ITimeMap> m_timeMap;
    bool inner_loop(capd::IOdeSolver & solver, capd::interval const & prevTime, capd::interval const T, std::vector<std::pair<capd::interval, capd::IVector>> & enclosures) const;

public:
    contractor_capd_full(box const & box, ode_constraint const * const ctr, bool const forward, unsigned const taylor_order, unsigned const grid_size, double const timeout = 0.0);
    void prune(box & b, SMTConfig & config) const;
    nlohmann::json generate_trace(box b, SMTConfig & config) const;
    std::ostream & display(std::ostream & out) const;
};

contractor mk_contractor_capd_simple(box const & box, ode_constraint const * const ctr, bool const forward);
contractor mk_contractor_capd_full(box const & box, ode_constraint const * const ctr, bool const forward, unsigned const taylor_order = 20, unsigned const grid_size = 16, double const timeout = 0.0);
}  // namespace dreal
