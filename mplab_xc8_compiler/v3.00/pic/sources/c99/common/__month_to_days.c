int __month_to_days(int month, int is_leap)
{
	static const int days_through_month[] = {
		0, 31, 59, 90,
		120, 151, 181, 212,
		243, 273, 304, 334 };
	int t = days_through_month[month];
	if (is_leap && month >= 2) t++;
	return t;
}
