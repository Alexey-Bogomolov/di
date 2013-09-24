//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct c1 { };

struct c2
{
    BOOST_DI_CTOR(c2, boost::shared_ptr<c1> sp) {
        if (sp) {
            std::clog << "in custom scope" << std::endl;
        } else {
            std::clog << "not in custom scope" << std::endl;
        }
    }
};

} // namespace

class custom_scope
{
public:
    class entry { };
    class exit { };

    template<typename TExpected, typename TGiven>
    class scope
    {
        class convertible
        {
        public:
            convertible(const boost::shared_ptr<TExpected>& object) // non explicit
                : object_(object)
            { }

            operator boost::shared_ptr<TExpected>() const {
                return object_;
            }

            boost::shared_ptr<TExpected> object_;
        };

    public:
        typedef convertible result_type;

        scope()
            : in_scope_(false) {
        }

        void call(const entry&) {
            in_scope_ = true;
        }

        void call(const exit&) {
            in_scope_ = false;
        }

        template<typename... Args>
        result_type create(Args&&... args) {
            if (in_scope_) {
                return boost::make_shared<TGiven>(std::forward(args)...);
            }
            return boost::shared_ptr<TGiven>();
        }

    private:
        bool in_scope_;
    };
};

int main()
{
    using injector_t = di::injector<
        di::scope<custom_scope>::bind<
            c1
        >
    >;;

    di::injector<injector_t> injector;

    injector.create<c2>(); // not in custom scope
    injector.call(custom_scope::entry());
    injector.create<c2>(); // in custom scope
    injector.call(custom_scope::exit());
    injector.create<c2>(); // not in custom scope
}

