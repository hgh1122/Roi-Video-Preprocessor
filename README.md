# ROI-based Video Pre-processor made by C

### What is this?
압축 효율을 높이기위해 픽셀을 8*8로 묶어 같은 값으로 양자화함

#### requirements 
./PeopleOnStreet_1280x720_30_Original.yuv

##### How to use?
1.  원본  YUV 파일을 프로젝트 루트 폴더에 위치
2.  소스 코드를 컴파일 후 실행한다.
3.  생성된 'PeopleOnStreet_1280x720_30_ExtractY'와 'PeopleOnStreet_1280x720_30_Original.yuv'를  YUV_viewer에서 사양에 맞춰 재생하고 결과물을 비교한다.

### 실험 결과 분석

다음 표는 8x8블럭 기준 1번 ROI내부(선명한 이미지 부분) 와 2번 ROI외부(모자이크 처리된 배경 부분)을 
단편적으로 보여주는 C코드 터미널 출력 예시이다 

#### 1. Inside ROI (Target Object)
| High Variance & High Entropy |
|:---:|
| `238 223 146  81  71  69  71  70` |
| `166 123  78  61  69  72  74  74` |
| ` 70  63  67  72  74  74  73  73` |
| `... (Values are irregular and complex)` |
<img width="413" height="183" alt="image" src="https://github.com/user-attachments/assets/4e2d9da4-2eb8-4c2d-88d6-694c44545675" />

#### 2. Outside ROI (Processed Background)
| Low Variance & Zero AC Coefficients |
|:---:|
| ` 57  57  57  57 |  57  57  57  57 ` |
| ` 57  57  57  57 |  57  57  57  57 ` |
| `----------------|----------------` |
| ` 79  79  79  79 |  56  56  56  56 ` |
| *The grid boundaries are clearly visible at the 8x8 alignment.*
<img width="441" height="186" alt="image" src="https://github.com/user-attachments/assets/40318d80-7461-41ee-a966-091f8a78d518" />

다음 ROI외부는 57,79,56으로 양자화값이 다르게 출력되어 들수있는 의문인 "8x8블럭을 각 블럭 첫 픽셀을 앵커 대표값으로 설정후 그 블럭 전체 픽셀을 앵커값으로 세팅했는데
왜 값이 다르게 나오냐?"라고 질문 할수있다. 해당 격자 좌표는 각 4개의 격자가 겹치는 구역을 출력한것으로 각 대표값에 의해 경계선이 생성된것을 보면 정상적으로 실행됬음을 알수있다.

<img width="760" height="444" alt="image" src="https://github.com/user-attachments/assets/5e10e9d3-dc28-4192-84db-d066c4fe7ddc" />
<격자가 겹치는 곳의 출력 영역 예시>

해당 작업을 통해 YUV파일에 일종의 양자화 작업을 완료 하였다. 그렇다면 파일의 크기를 확인해보자
<img width="603" height="55" alt="image" src="https://github.com/user-attachments/assets/aba6817c-3f41-46c0-86ab-cc71cc2f6018" />
<각 생성된 파일의 크기>

당연하게도 원래 yuv파일의 크기는 바뀐점이 없다, 하지만 압축을 하였을때는 양자화 작업의 효과가 나타난다.
<img width="769" height="81" alt="image" src="https://github.com/user-attachments/assets/947762dd-e018-43f3-8ec6-3af4756a054a" />
<각 zip파일로 압축 후 비교>

기존 파일은 40,500 -> 27,892 으로 약 68.9%의 압축률을 보여준다
양자화 후 파일은 40,500 -> 9,917 으로 약 24.5%으로 저장 공간을 2.8배나 더 아낄 수 있다.
해당 방식을 그대로 응용하여 구현할 순 없겠지만 네트워크 품질에 따라 인코딩파일을 가변해서 중요한 부분만 선명하게 송출할수있는 서비스의 가능성을 단편적으로 보여주고 있다.


