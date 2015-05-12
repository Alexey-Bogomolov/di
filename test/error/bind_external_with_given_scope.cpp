//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

test error_bind_external_with_given_scope = [] {
    di::make_injector(
        di::bind<int>.in(di::unique).to(42)
    );
};

