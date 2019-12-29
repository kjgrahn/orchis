#include <split.h>

#include <orchis.h>

namespace splitjoin {

    using orchis::TC;

    void assert_splits(const std::string& s, const std::string& ref)
    {
	const auto v = split(s);
	orchis::assert_eq(join('+', v), ref);
    }

    void assert_splits(const std::string& s, size_t n, const std::string& ref)
    {
	const auto v = split(s, n);
	orchis::assert_eq(join('+', v), ref);
    }

    void assert_splits(const char* delim,
		       const std::string& s, const std::string& ref)
    {
	const auto v = split(delim, s);
	orchis::assert_eq(join('+', v), ref);
    }

    void empty(TC)
    {
	assert_splits("", "");
    }

    void one(TC)
    {
	assert_splits("foo", "foo");
    }

    void two(TC)
    {
	assert_splits("foo bar",  "foo+bar");
	assert_splits("foo  bar", "foo+bar");
	assert_splits("foo\tbar", "foo+bar");
	assert_splits("foo\nbar", "foo+bar");
    }

    void three(TC)
    {
	assert_splits("foo bar baz", "foo+bar+baz");
    }

    void trailing(TC)
    {
	assert_splits(" foo bar baz\t ", "foo+bar+baz");
    }

    void limited(TC)
    {
	const std::string s = "  foo  bar  baz  bat  ";
	assert_splits(s, 9, "foo+bar+baz+bat");
	assert_splits(s, 5, "foo+bar+baz+bat");
	assert_splits(s, 4, "foo+bar+baz+bat");
	assert_splits(s, 3, "foo+bar+baz  bat");
	assert_splits(s, 2, "foo+bar  baz  bat");
	assert_splits(s, 1, "foo  bar  baz  bat");
	assert_splits(s, 0, "foo+bar+baz+bat");
    }

    void delimited(TC)
    {
	assert_splits("::", "", "");
	assert_splits("::", "foo", "foo");
	assert_splits("::", "foo::bar", "foo+bar");
	assert_splits("::", "foo::bar::baz", "foo+bar+baz");
	assert_splits("::", "::foo", "+foo");
	assert_splits("::", "foo::", "foo+");
	assert_splits("::", "foo::::bar", "foo++bar");
	assert_splits("::", "::", "+");
    }
}
