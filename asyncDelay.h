#ifndef asyncDelay_h
#define asyncDelay_h

typedef void (*GeneralFunction) ();

class asyncDelay{
	public:
		asyncDelay(int refresh);
		void runJob(GeneralFunction);
	private:
		int _old, _refresh;
};

#endif