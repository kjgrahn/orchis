#include <namespace.h>

#include <orchis.h>
#include <memory>

namespace {

    using orchis::TC;
    using orchis::assert_eq;

    std::unique_ptr<Namespace> make_unique(std::ostream& os)
    {
	return std::unique_ptr<Namespace>{new Namespace{os}};
    }

    void write(std::ostream& os, Namespace& ns, const std::vector<std::string>& nss)
    {
	os << ns.open(nss);
	os << "sth;\n";
    }
}

namespace nspace {

    void empty(TC)
    {
	std::ostringstream oss;
	auto p = make_unique(oss);

	p.reset(nullptr);
	assert_eq(oss.str(), "");
    }

    void simple(TC)
    {
	std::ostringstream oss;
	auto p = make_unique(oss);

	write(oss, *p, {"foo"});
	p.reset(nullptr);
	assert_eq(oss.str(),
		  "namespace foo {\n"
		  "    sth;\n"
		  "}\n");
    }

    void open(TC)
    {
	std::ostringstream oss;
	auto p = make_unique(oss);

	write(oss, *p, {});
	write(oss, *p, {"foo"});
	write(oss, *p, {"foo", "bar"});
	p.reset(nullptr);
	assert_eq(oss.str(),
		  "sth;\n"
		  "namespace foo {\n"
		  "    sth;\n"
		  "    namespace bar {\n"
		  "\t"    "sth;\n"
		  "    }\n"
		  "}\n");
    }

    void reopen(TC)
    {
	std::ostringstream oss;
	auto p = make_unique(oss);

	write(oss, *p, {"foo", "bar"});
	write(oss, *p, {"foo", "baz"});
	write(oss, *p, {"bat", "fred"});
	write(oss, *p, {});
	p.reset(nullptr);
	assert_eq(oss.str(),
		  "namespace foo {\n"
		  "    namespace bar {\n"
		  "\t"    "sth;\n"
		  "    }\n"
		  "    namespace baz {\n"
		  "\t"    "sth;\n"
		  "    }\n"
		  "}\n"
		  "namespace bat {\n"
		  "    namespace fred {\n"
		  "\t"    "sth;\n"
		  "    }\n"
		  "}\n"
		  "sth;\n");
    }
}
