# SystemMemoryLog
* 기존 로그와 달리 중간중간마다 로그를 파일로 저장하거나 콘솔에 프린트하지 않고 메모리에 쌓아둡니다.
* 특정 시점에서 한 번에 결과값을 출력하는 함수를 통해 로그를 확인합니다.
* 코드의 동작 흐름을 파악하는데 사용됩니다.

사용방법
1. 클래스를 선언한 뒤, SystemMemoryLog클래스를 상속받습니다.
2. 상속을 받은 클래스에서 열거형을 선언합니다. 열거형의 갯수는 본인이 남기고싶은 로그 종류의 개수만큼 선언합니다. 처음 시작값은 0 이여야합니다.
3. SetLogOption()함수를 통해 로그 옵션을 설정합니다.

      ### ```SetLogOption(int sizeOfLog, int maxCount, int extraParamCount, ...);```
    
    각 매개변수의 의미는 다음과 같습니다.
    * int sizeOfLog : 로그의 종류 개수입니다.
    * int maxCount : 저장할 로그의 최대 개수입니다. maxCount를 2000 으로 설정하였을 경우, 가장 최근에 남겨진 2000개의 로그가 저장됩니다.
    * int extraParamCount : 로그를 남길 여분의 정수값의 개수입니다. 해당 코드지점에서 확인하고 싶은 변수들의 값들을 여기서 설정한 개수만큼 저장할 수 있습니다. 모든 지점에 지정한 개수만큼 값이 들어가야합니다.
    * ... : 가변인자로 const WCHAR* 문자열 포인터가 들어갑니다. sizeOfLog에서 지정한 개수만큼 문자열들을 넣으면 됩니다. 이 문자열은 선언한 열거형과 같은 이름(혹은, 비슷한 이름), 같은 순서로 넣어야합니다.
    
4. 콘솔출력 혹은 파일출력 함수를 선언 및 정의합니다. SetLogOption에 매개변수로 넣은 문자열들은 logTypeArray 2차원 배열에 순차적으로 저장되어 있습니다. 로그로 남긴 문자열들의 인덱스는 logIdxArray에 저장되어 있습니다. 최신의 인덱스는 currentIndex를 참조하면됩니다. currentIndex % maxCount + 1 로 접근하세요.
5. AddLog()함수를 통해 로그를 추가합니다.

      ### ```AddLog(int log, ...);```
      
     각 매개변수의 의미는 다음과 같습니다.
     * int log : 로그의 종류입니다.
     * 앞서 SetLogOption에서 정의한 extraParamCount 만큼 int형을 넣으면됩니다.
     
6. 로그수집이 완료되었다면, 직접 정의한 출력함수를 호출합니다.
7. 로그수집에 사용되었던 메모리들을 해제하려면 ReleaseLog()를 호출하세요.

      ### ```ReleaseLog();```
      
## 사용 예
    class MyLogClass : public CSystemMemoryLog
    {
        enum LOG
        {
            enum1,
            enum2,
            enum3
        }
        
        void Print()
        {
            for(int i = 0 ; i < currentIndex % maxCount + 1; i++)
            {
                wprintf(L"%s %d %d %d\n", logTypeArray[logIdxArray[i]], extraParamArray[0][i], extraParamArray[1][i], extraParamArray[2][i]); 
            }
        }
    }
    
    int main()
    {
        MyLogClass myLog;
        
        myLog.SetLogOption(3, 2000, 3, L"enum1", L"enum2", L"enum3");
        
        myLog.AddLog(MyLogClass::enum1, 1, 2, 3);
        
        myLog.AddLog(myLogClass::enum2, 4, 5, 6);
        
        myLog.AddLog(myLogClass::enum3, 7, 8, 9);
        
        myLog.Print();
        
        myLog.ReleaseLog();
        
        return 0;
    }
