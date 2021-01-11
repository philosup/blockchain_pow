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
- OpenSSL 1.1.1f  31 Mar 2020
    > sudo apt-get install libssl-dev openssl

- docker

## 작업내역
- 블록 클래스
- 블록체인 클래스 
- 해쉬
- 난위도
- 노드 - 마이닝, 네트워크, 검증
- 도커 테스트
