# blockchain_pow

## 1. 작업증명(Proof-of-Work) 과정 구현하기

비트코인 블록 header 자료 구조를 구현하고 (트랜잭션 목록 및 머클 트리는 구현 불필요, 빈 블록으로 가정), nonce 값을 변화시키며 유효한 블록 hash를 탐색하는 채굴 과정을 구현 (블록 10개 생성)

## 2. 경쟁적 채굴 과정 구현하기 (OPTIONAL)

2개의 worker가 경쟁적으로 채굴 과정을 수행하며, 한 worker는 lower bound부터 1씩 nonce 값을 증가, 다른 worker는 upper bound로부터 1씩 nonce 값을 감소시키는 전략을 취함 (다른 전략을 구현할 수 있는 경우 선택 가능)
두 worker 중 한 worker가 먼저 유효한 블록 hash를 발견하면 이를 다른 worker에게 전달하고 이를 전달받은 worker는 유효성을 검증한 이후, 다음 블록 채굴 시작
worker간 통신은 프로세스 간 통신이나 소켓 통신 등 자유롭게 선택 가능




# 개발환경

- Windows 10 WSL2 - Ubuntu 20.04.1 LTS
- Visual Studio Code
  - C/C++ https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
  - C++ Intellisense
  - C/C++ Compile Run
  - Better C++ Syntax
  - C++ Helper
  - C/C++ Themes

- C++ - g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

## 작업내역
- 블록 클래스
- 블록체인 클래스 
- 해쉬
- 난위도

### 변경
- http서버 방식으로 동일 실행파일로 다른 포트 연결하는 방식으로
  - 여러 이유로 시간 투자의 어려움이 있어 빨리 구현하는 방향으로 선회.

### 추후
- 노드 - 마이닝, 네트워크, 검증
- 도커 테스트
- gRPC or p2p

## 라이브러리
- httplib - https://github.com/yhirose/cpp-httplib
- rapidjson - https://github.com/Tencent/rapidjson/


## 컴파일 및 실행

실행할때 마다 8080포트 부터 1씩 증가하면서 포트 셋팅하도록 설정되어 있습니다.

>g++ -g main.cpp -o main -lpthread

> ./start_server.sh

> ./stop_server.sh


- 경쟁적 채굴 시작
> curl -d '{"difficulty":19, "begin":0, "end":10000000}' -H "Content-Type: application/json" -X POST http://localhost:8080/mine_all

- 종료
> curl -d '' -H "Content-Type: application/json" -X POST http://localhost:8080/exit

> pkill main



```sh
./start_server.sh
2
Http Server Start !!
Listen : 8080
Hello World! : 8080
2
Http Server Start !!
add port : 8081
Listen : 8081
Hello World! : 8080
Hello World! : 8081
2
Http Server Start !!
add port : 8082
add port : 8082
Listen : 8082
set difficulty : 19
start mining !!!start mining : 0, 8081, 10000000~20000000
start mining : 0, 8080, 0~10000000
start mining : 0, 8082, 20000000~30000000
mined : 8082, 20014908
{"index":1,"timestamp":19,"prevhash":"a28048d4caf12f50c924a4dc7239b3f3fee68c1582acdab683f91cc7dd232f56","nonce":20014908,"hash":"0000167fbfe42a5b344786b905198c9a8b19d528b3d8fab9a0d43b0f2a6e9cb1"}
start mining : 1, 8080, 10000000~20000000
start mining : 1, 8081, 20000000~30000000
start mining : 1, 8082, 0~10000000
mined : 8082, 70125
{"index":2,"timestamp":19,"prevhash":"0000167fbfe42a5b344786b905198c9a8b19d528b3d8fab9a0d43b0f2a6e9cb1","nonce":70125,"hash":"0000143bac7856a1a2dd1d251165b24cb9a0b82f84523833c36cfaf87c44c8c0"}
start mining : 2, 8082, 0~10000000
start mining : 2, 8080, 10000000~20000000
start mining : 2, 8081, 20000000~30000000
mined : 8082, 207757
{"index":3,"timestamp":19,"prevhash":"0000143bac7856a1a2dd1d251165b24cb9a0b82f84523833c36cfaf87c44c8c0","nonce":207757,"hash":"00001ecf5d37baff1927ea4566f2bf77895db5a1c7984c4aac31236dc12cb19f"}
start mining : 3, 8082, 0~10000000
start mining : 3, 8081, 20000000~30000000
start mining : 3, 8080, 10000000~20000000
mined : 8081, 20062369
{"index":4,"timestamp":19,"prevhash":"00001ecf5d37baff1927ea4566f2bf77895db5a1c7984c4aac31236dc12cb19f","nonce":20062369,"hash":"00000cb9b45412fca08a0912df3ccd21215b945ab593dc659eeb1585a1a3b5fc"}
start mining : 4, 8081, 0~10000000
start mining : 4, 8080, 10000000~20000000
start mining : 4, 8082, 20000000~30000000
mined : 8080, 10344968
{"index":5,"timestamp":19,"prevhash":"00000cb9b45412fca08a0912df3ccd21215b945ab593dc659eeb1585a1a3b5fc","nonce":10344968,"hash":"000005151e71806b163b06ba524a1b9d6e4ceb0a7f17c97e359df503074431f0"}
```