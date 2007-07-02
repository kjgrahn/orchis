#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <string>
#include <unistd.h>

#include <testicle.h>

namespace {

    class Progress {
    public:
        Progress(std::ostream& os, bool verbose, int indent)
	    : os_(os),
	      verbose_(verbose),
              indent_(indent),
	      pass_(0),
	      fail_(0),
	      error_(0)
	{}

	void begin(const char * name);
	void pass();
	void fail(const testicle::AssertionError& error);
	void error();
	void done();
	int rc() const { return fail_+error_!=0; }

    private:
	struct Problem {
	    Problem(const std::string& nam,
		    const testicle::AssertionError& err)
		: name(nam),
		  error(err)
	    {}
	    std::string name;
	    testicle::AssertionError error;
	};

	std::ostream& os_;
	const bool verbose_;
        const int indent_;
	int pass_;
	int fail_;
	int error_;
	std::string current_;
	std::vector<Problem> problems_;
    };

    void Progress::begin(const char * name)
    {
	current_ = name;
	if(verbose_) {
           os_ << name
               << std::string(indent_ - std::strlen(name), ' ')
               << std::flush;
	}
    }

    void Progress::pass()
    {
	pass_++;
	if(verbose_) {
	    os_ << "ok" << std::endl;
	}
	else {
	    os_ << '.' << std::flush;
	}
    }

    void Progress::fail(const testicle::AssertionError& error)
    {
	fail_++;
	if(verbose_) {
	    os_ << "FAIL" << std::endl;
	}
	else {
	    os_ << 'F' << std::flush;
	}

	problems_.push_back(Problem(current_, error));
    }

    void Progress::error()
    {
	error_++;
	if(verbose_) {
	    os_ << "ERROR" << std::endl;
	}
	else {
	    os_ << 'E' << std::flush;
	}
    }

    void Progress::done()
    {
	static const char hr[] = "----------------------------------------"
	                         "------------------------------\n";

	if(!verbose_) {
	    os_ << '\n';
	}
	os_ << hr;

	for(std::vector<Problem>::const_iterator i = problems_.begin();
	    i != problems_.end();
	    ++i) {
	    os_ << i->name << '\n'
		<< i->error << '\n'
		<< hr;
	}

	if(fail_+error_ == 0) {
	    os_ << "OK (" << pass_ << " tests)\n";
	}
	else {
	    int total = pass_ + fail_ + error_;
	    os_ << "FAILED (" << total << " tests; "
		<< fail_ << " FAIL, "
		<< error_ <<" ERROR)\n";
	}
    }

    struct Entry {
	char * name;
	void (*f)();
    };


    class NonMatch {
    public:
	NonMatch(char ** begin, char ** end)
	    : begin_(begin),
	      end_(end)
	{}

	bool operator() (const Entry& e) const;

    private:
	char ** const begin_;
	char ** const end_;
    };

    bool NonMatch::operator() (const Entry& e) const
    {
	char ** i = begin_;
	while(i!=end_) {
	    if(std::strstr(e.name, *i)==e.name) return false;
	    ++i;
	}
	return true;
    }
}

const int primes_a[] = {1, 3, 5, 7 };
const std::vector<int> primes(primes_a, primes_a+4);
const char * const primes_s = "1357";

void pass_a() { testicle::assert_(true); }
void pass_a_eq() { testicle::assert_eq(1, 1+0); }
void pass_a_neq() { testicle::assert_neq(1, 1+1); }
void pass_a_lt() { testicle::assert_lt(1, 1+1); }
void pass_a_gt() { testicle::assert_gt(1+1, 1.5); }
void pass_a_in() { testicle::assert_in(primes, 5); }
void pass_a_in2() { testicle::assert_in(primes_s, primes_s+4, '5'); }
void pass_a_not_in() { testicle::assert_not_in(primes, 9); }
void pass_a_not_in2() { testicle::assert_not_in(primes_s, primes_s+4, '9'); }

void fail_a() { testicle::assert_(!"foo"); }
void fail_a_eq() { testicle::assert_eq(1, 1+1); }
void fail_a_neq() { testicle::assert_neq(1, 1+0); }
void fail_a_lt() { testicle::assert_lt(1+2, 1+1); }
void fail_a_gt() { testicle::assert_gt(1.5, 3); }
void fail_a_in() { testicle::assert_in(primes, 9); }
void fail_a_in2() { testicle::assert_in(primes_s, primes_s+4, '9'); }
void fail_a_not_in() { testicle::assert_not_in(primes, 5); }
void fail_a_not_in2() { testicle::assert_not_in(primes_s, primes_s+4, '5'); }


int main(int argc, char ** argv)
{
    const char * const prog = argv[0];

    static const Entry entries[] = {
	{ "pass_a", pass_a },
	{ "pass_a_eq", pass_a_eq },
	{ "pass_a_neq", pass_a_neq },
	{ "pass_a_lt", pass_a_lt },
	{ "pass_a_gt", pass_a_gt },
	{ "pass_a_in", pass_a_in },
	{ "pass_a_in2", pass_a_in },
	{ "pass_a_not_in", pass_a_not_in },
	{ "pass_a_not_in2", pass_a_not_in },
	{ "fail_a", fail_a },
	{ "fail_a_eq", fail_a_eq },
	{ "fail_a_neq", fail_a_neq },
	{ "fail_a_lt", fail_a_lt },
	{ "fail_a_gt", fail_a_gt },
	{ "fail_a_in", fail_a_in },
	{ "fail_a_in2", fail_a_in },
	{ "fail_a_not_in", fail_a_not_in },
	{ "fail_a_not_in2", fail_a_not_in },
    };

    static const size_t NENTRIES = sizeof(entries)/sizeof(entries[0]);

    bool verbose = false;
    size_t indentation = 0;
    bool just_list = false;
    bool bork_on_fail = false;

    int ch;
    while((ch = getopt(argc, argv, "+vlc")) != -1) {
	switch(ch) {
	case 'v':
	    verbose = true;
            for(size_t i=0; i<NENTRIES; ++i) {
                indentation = std::max(indentation,
                                       std::strlen(entries[i].name) + 1);
            }
	    break;
	case 'l':
	    just_list = true;
	    break;
	case 'c':
	    bork_on_fail = true;
	    break;
	case '?':
	default:
	    std::cerr << "usage: " << prog << " [-vc] [pattern ...]\n"
		      << "       " << prog << " -l [pattern ...]\n";
	    return 0;
	}
    }

    std::vector<Entry> chosen(entries, entries+NENTRIES);
    if(optind!=argc) {
	chosen.erase(std::remove_if(chosen.begin(), chosen.end(),
				    NonMatch(argv+optind, argv+argc)),
		     chosen.end());
    }

    if(just_list) {
	for(std::vector<Entry>::const_iterator i = chosen.begin();
	    i!=chosen.end();
	    ++i) {
	    std::cout << i->name << '\n';
	}
	return 0;
    }

    Progress p(std::cout, verbose, indentation);

    for(std::vector<Entry>::const_iterator i = chosen.begin();
	i!=chosen.end();
	++i) {

	const Entry& e = *i;

	p.begin(e.name);

	if(bork_on_fail) {
	    e.f();
	    p.pass();
	}
	else {
	    try {
		e.f();
		p.pass();
	    }
	    catch(testicle::AssertionError& err) {
		p.fail(err);
	    }
	    catch(...) {
		p.error();
	    }
	}
    }
    p.done();

    return p.rc();
}
