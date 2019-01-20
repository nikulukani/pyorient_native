/* Sam Caldwell <mail@samcaldwell.net> (released public domain to the project where this code resides).
 * This file contains the tests for the version_compare.h source file.
 */
#include "../pyorient_native/version_compare.h"
#include <assert.h>
#include <iostream>

/*void show_values(Version v1, Version v2){
	cout << "show_values():\n";
	cout << "\tv1: " << v1.major << "." << v1.minor << "." << v1.revision << "." << v1.build << "\n";
	cout << "\tv2: " << v2.major << "." << v2.minor << "." << v2.revision << "." << v2.build << "\n";
	cout << "\t---\n";
}*/

bool test_equivalence_operator_happy(){
	cout << "test: test_equivalence_operator_happy() starting\n";
	const char *version_string = "1.0.0";

	Version v1(version_string);
	Version v2(version_string);

	return (v1 == v2);
}

bool test_equivalence_operator_sad(){
	cout << "test: test_equivalence_operator_sad(): starting\n";
	const char *version_string1 = "1.0.0";
	const char *version_string2 = "2.0.0";

	Version v1(version_string1);
	Version v2(version_string2);

	return !(v1 == v2);
}

bool test_less_than_operator_happy(){
	cout << "test: test_less_than_operator_happy(): starting\n";
	const char *version_string1 = "1.0.0";
	const char *version_string2 = "1.0.1";

	Version v1(version_string1);
	Version v2(version_string2);

	return (v1 < v2);
}

bool test_less_than_operator_sad_if_equal(){
	cout << "test: test_less_than_operator_sad_if_equal(): starting\n";
	const char *version_string1 = "1.0.0";
	const char *version_string2 = "1.0.0";

	Version v1(version_string1);
	Version v2(version_string2);

	return !(v1 < v2);
}

bool test_less_than_operator_sad_if_greater(){
	cout << "test: test_less_than_operator_sad_if_greater(): starting\n";
	const char *version_string1 = "2.0.0";
	const char *version_string2 = "1.0.0";

	Version v1(version_string1);
	Version v2(version_string2);

	return !(v1 < v2);
}

bool test_inequal_operator_happy(){
	cout << "test: test_less_than_operator_sad_if_greater(): starting\n";
	const char *version_string1 = "2.0.0";
	const char *version_string2 = "1.0.0";

	Version v1(version_string1);
	Version v2(version_string2);

	return (v1 != v2);
}

bool test_inequal_operator_sad(){
	cout << "test: test_less_than_operator_sad_if_greater(): starting\n";
	const char *version_string1 = "1.0.0";
	const char *version_string2 = "1.0.0";

	Version v1(version_string1);
	Version v2(version_string2);

	return !(v1 != v2);
}

bool test_casted_comparison_happy(){
	cout << "test: test_casted_comparison_happy(): starting\n";
	return (Version)("1.0") == (Version)("1.0");
}
bool test_casted_comparison_sad(){
	cout << "test: test_casted_comparison_sad(): starting\n";
	return (Version)("1.0") == (Version)("1.0");
}

int main(){
	cout << "\n\nStarting tests.\n";

	assert (test_equivalence_operator_happy());
	assert (test_equivalence_operator_sad());
	assert (test_less_than_operator_happy());
	assert (test_less_than_operator_sad_if_equal());
	assert (test_less_than_operator_sad_if_greater());
	assert (test_casted_comparison_happy());
	assert (test_casted_comparison_sad());
}
