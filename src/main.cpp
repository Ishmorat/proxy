#include "proxy.h"

void test(int argc, char* argv[]) {
	Config cfg = Config::create(argc, argv);
	cfg.print();
	Proxy pr(cfg);
	pr.run();
}

int main(int argc, char* argv[]) {
	test(argc, argv);
	return 0;
}
