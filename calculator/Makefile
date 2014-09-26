#CXX=/opt/local/bin/x86_64-apple-darwin13-g++-mp-4.9
#CFLAG=-std=c++11 -stdlib=libstdc++

CXX=clang++ 
CFLAG=-std=c++11 -stdlib=libc++ -I$(GMOCK_HOME)/include -I$(GMOCK_HOME)/gtest/include
CFLAG+=-L$(GMOCK_HOME)/mybuild -L$(GMOCK_HOME)/gtest/mybuild
CFLAG+=-DGTEST_USE_OWN_TR1_TUPLE=1
input="calc2.cc"
output="Calc"


default:
	@$(CXX) $(CFLAG) $(input) -o $(output)
run: 
	@./$(output)
clean:
	rm ./$(output)
