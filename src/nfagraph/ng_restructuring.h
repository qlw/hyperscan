/*
 * Copyright (c) 2015, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \file
 * \brief State numbering and late graph restructuring code.

 */
#ifndef NG_RESTRUCTURING_H
#define NG_RESTRUCTURING_H

#include "ng_holder.h"
#include "ue2common.h"
#include "util/ue2_containers.h"

#include <map>
#include <vector>

namespace ue2 {

class NGHolder;

/** Construct a reversed copy of an arbitrary NGHolder, mapping starts to
 * accepts. */
void reverseHolder(const NGHolder &g, NGHolder &out);

/** Connect the start vertex to each of the vertices in \p tops. This is useful
 * temporarily for when we need to run a graph algorithm that expects a single
 * source vertex. */
void wireStartToTops(NGHolder &g, const std::map<u32, NFAVertex> &tops,
                     std::vector<NFAEdge> &topEdges);

/**
 * \brief Special state index value meaning that the vertex will not
 * participate in an (NFA/DFA/etc) implementation.
 */
static constexpr u32 NO_STATE = ~0;

/**
 * \brief Gives each participating vertex in the graph a unique state index.
 */
ue2::unordered_map<NFAVertex, u32>
numberStates(NGHolder &h,
             const std::map<u32, NFAVertex> &tops = std::map<u32, NFAVertex>{});

/**
 * \brief Counts the number of states (vertices with state indices) in the
 * graph.
 *
 * If addTops is true, also accounts for states that will be constructed for
 * each unique top.
 */
u32 countStates(const NGHolder &g,
                const ue2::unordered_map<NFAVertex, u32> &state_ids,
                bool addTops = true);

/** Optimisation: drop unnecessary start states. */
void dropUnusedStarts(NGHolder &g, ue2::unordered_map<NFAVertex, u32> &states);

/**
 * \brief Returns a set of vertices that will not participate in an
 * implementation (NFA, DFA etc) of this graph. For example, starts with no
 * successors.
 */
flat_set<NFAVertex> findUnusedStates(const NGHolder &g);

} // namespace ue2

#endif
