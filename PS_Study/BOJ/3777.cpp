#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string s[1001] = { "0", "0", "1", "1", "3", "5"};	// default Sequence
int n;

// Add function
string Add(string &s1, string &s2, string &chk)
{
	// resut 변수 초기화
	string result(max(s1.size(), s2.size()), '0');
	bool carry = false;	// 올림 여부 확인
	int out = 0;	/// 내림 여부 확인

	for (unsigned int i = 0; i < result.size(); i++)
	{
		int temp = carry + out;
		carry = false;
		out = 0;

		// 일의 자리 수 부터 연산, - '0' 은 문자열을 숫자형으로 변환해주기 위함
		if (i < s1.size())
			temp += s1[s1.size() - i - 1] - '0';
		if (i < s2.size())
			temp += s2[s2.size() - i - 1] - '0';

		// 짝수-홀수 번째에 따라 일의자리의 +-1 결정
		if (i == 0 && chk == "plus") {
			temp++;
		}
		else if (i == 0 && chk == "min") {
			temp--;
		}
		else {}

		// temp가 10 이상일 경우 올림
		if (temp >= 10) {
			carry = true;
			temp -= 10;
		}

		// temp가 0 일 경우 내림
		if (temp < 0) {
			out = -1;
			temp += 10;
		}

		// 해당 자리수에 문자형으로 변환된 temp를 저장
		result[result.size() - i - 1] = temp + '0';
	}

	// 마지막에 올림이 있을 경우 result의 맨 앞에 '1' 삽입
	if (carry)
		result.insert(result.begin(), '1');

	return result;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL), cout.tie(NULL);

	// 공식 적용 : 홀수 시퀀스 = 2(Sn-1)-1, 짝수 시퀀스 = 2(Sn-1)+1
	string min = "min";
	string plus = "plus";

	for(int i=6; i<=1001; i++){
		if (i % 2 == 1) {
			s[i] = Add(s[i - 1], s[i - 1], min);	// 홀수
		}
		else {
			s[i] = Add(s[i - 1], s[i - 1], plus);	// 짝수
		}
	}

	while (cin >> n) {
		cout << s[n] << '\n';
	}

	return 0;
}

/*
한 자리 숫자로 구성된 시퀀스, 숫자 1은 처음에는 컴퓨터에 기록됩니다.
연속된 각 시간 단계에서 컴퓨터는 각 숫자 0을 시퀀 스 1 0으로, 각 숫자 1을 시퀀스 0 1로 동시에 변환한다.

첫 번째 단계 이후, 시퀀스 0 1을 얻는다.
제2 후, 서열 1 0 0 1은 제3 후, 서열 0 1은 1 0 1 0 0 0 0 1 등이 된다.

n 단계 후에 시퀀스에 몇 쌍의 연속 0이 나타날 것인가?

숫자 0을 시퀀스 1 0 으로,
숫자 1을 시퀀스 0 1 로 변환

첫 번째 단계, 시퀀스 0 1 (1)
두 번째 단계, 시퀀스 1 0 0 1 (01)
세 번쨰 단계, 시퀀스 0 1 1 0 1 0 0 1 (1001)
*/

/*
[접근]
숫자 0을 시퀀스 1 0 으로,
숫자 1을 시퀀스 0 1 로 변환.

규칙을 찾아본다.
S1 = 0 1  -> 0개
S2 = 1 0 0 1  -> 1개
S3 = 0 1 1 0 1 0 0 1 -> 1개
S4 = 1 0 0 1 0 1 1 0 0 1 1 0 1 0 0 1  -> 3개
S5 = 0 1 1 0 1 0 0 1 1 0 0 1 0 1 1 0 1 0 0 1 0 1 1 0 0 1 1 0 1 0 0 1  -> 5개
S6 = 1001011001101001011010011001011001101001100101101001011001101001 -> 11개
S7 = ... -> 21개

Sn에 '0 1' 이 있을 경우, Sn+1에서 '0 0' 이 생성된다.
우선 이렇게 S6 = '0 0' 이 11개 있음을 알 수 있다.

[시도]
0 1 1 3 5 11 ... 여기서 규칙을 찾아봐야 할 것 같다..
-> 아니다, 시퀀스에서 규칙을 찾을 수 있을 것 같다.

EX)
S2 : 1 0 0 1
S3 : 0 1 1 0 "1 0 0 1"
S3은 S2의 시퀀스를 포함하고 있다. 그리고 그 앞 시퀀스는 Sn-1의 not이다.
여기서 확인할 점은, Sn-1에서의 '0 1' 개수와 not(Sn-1)에서의 '0 1'개수가 같다는 점.
그리하여 Sn = 2(Sn-1) 라는 식을 도출할 수 있었다. (Sn-1 에서의 '0 1'은 Sn 에서의 '0 0'과 같으므로)

하지만, 저 식으로는 규칙을 만들어낼 수 없었다. 
다시 시퀀스를 들여다보니 짝수번째 시퀀스는 Sn-1과 not(Sn-1)의 교차가 '0 0'이고,
홀수번째 시퀀스는 교차가 '0 1'인 것을 확인하였다.
즉, 짝수는 이전 시퀀스(홀수번째)에서의 '0 1'이 '1 0 0 1'으로 바뀌므로 + 1을 해주고,
홀수는 이전 시퀀스(짝수번째)에서의 '0 0'이 '1 0 1 0'으로 바뀌므로 - 1을 해주면 된다.

결론으로,
홀수번째 시퀀스 = 2(Sn-1)-1
짝수번째 시퀀스 = 2(Sn-1)+1

[배운점]
연산 중 long long의 범위를 초과하여 더 큰 수를 다루는 자료형이 필요했다.
하지만 알고 있는 가장 큰 자료형이 unsigned long long 자료형이었기 때문에.. 구글링이 필요했다.

Bigint라는 라이브러리도 있었는데, 
다른 방법을 찾아보다 문자열을 활용한 연산이 가능하다는 것을 알게 되었다.

결과는 여러 방법으로 비교해도 같게 나오는데 서버에만 제출하면 '틀렸습니다.' 처리가 되어서
계속 코드를 뒤집어 엎어보기도하고 계속 디버깅 돌려보고 했는데 출력형식에 문제가 있었다.
이 문제같은 경우는 테스트케이스가 주어지지 않아서 계속해서 입력이 들어오면 결과를 개행해주면서
출력해주어야 했다. 
*/