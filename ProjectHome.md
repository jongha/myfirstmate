# 프로젝트 목표 #
본 프로젝트의 목표는 인터넷 자료들을 검색하고 원하는 수많은 정보 중 원하는 정보만 선택하여 자신만의 영역에 저장하고 언제든지 원할 때 다시 조회, 수정, 삭제 할 수 있는 기능의 프로그램을 구현하는 것이다. 프로젝트의 목표에 해당하는 핵심 기능과 핵심을 보조하는 부가 기능을 정리하면 다음과 같다.

## 프로젝트 목표의 핵심 기능 ##
  * 인터넷의 정보의 스크랩 기능
  * IE Toolbar를 이용한 회원 로그인
  * 상대경로의 데이터처리 기능
  * 자신만의 영역에 저장하는 기능
  * 등록된 자료의 조회, 수정, 삭제 기능


## 현 진행 사항 ##
본 프로젝트의 처음에 목표로 생각했던 사항들은 기능적으로는 모두 완료 되었지만 아직도 보완해야 할 과제가 남아있다. 마우스 오른쪽 버튼으로 막아놓은 사이트에서도 스크랩 기능을 사용할 수 있도록 프로그램을 개선하고, script를 이용해 페이지를 생성할 경우 제대로 스크랩되지 않는 점, 프로젝트의 Http 통신상의 보안 강화 목적으로 SSL등 암호화 방식의 도입, 아직 완벽히 매끄럽지 못한 사용자UI 의 개선, 또한 발견되지 않은 버그 등이 있다.

## 추후 진행 사항 ##
본 프로젝트의 배포와 사용자들의 커뮤니티를 위해 사이트를 개발 중이고 어느 정도 프로젝트가 안정된 후 위 사이트를 통해서 배포할 예정이다. 또한 이번 수업으로만 끝나는 프로젝트가 아니라 앞으로 위 사이트를 통해 사용자 Feedback을 받고 지속적으로 버전업을 해나가면서 관리해 나갈 것이다.


### 추후 개발 및 진행 사항 ###
  * 마우스 오른쪽 버튼 막아놓은 사이트에서도 스크랩 기능이 가능하도록 개선.
  * Script로 생성하는 웹 페이지에서도 깨지지 않고 스크랩 할 수 있도록 개선.
  * 현 개발 환경에만 설치되 있는 서버프로그램을 서비스 환경에 설치 및 테스트.
  * SSL(Secure Sockets Layer) 적용
  * HTTP의 응답메시지의 AES(Advanced Encryption Standard) 암호화


# 프로젝트 구성 #
현 버전까지의 프로젝트 구성을 각 파트 별로 설명하고, 초기 프로젝트 제안 때와 변경된 사항은 없는지 있다면 그 이유에 대해 설명한다.

## Client (MFM Toolbar) ##
개발된 프로그램의 배포를 위해 Setup 프로그램의 사용이 추가되었다. Inno Setup 5를 사용하였고 Third Party Tool인 IsTool을 추가로 설치하여 Inno Setup의 설치 스크립트 작성을 비교적 쉽게 할 수 있었다.

### 개발 환경 ###
|운영체제|Windows (Internet Explorer 기반)|
|:-----------|:---------------------------------|
|Language|C++, Javascript|
|사용프로그램|Visual Studio .NET(2005)|
|Setup 프로그램|Inno Setup 5 + IsTool|

## Server (User Community) ##
서버 부분은 개발 환경과 서비스 환경으로 나뉘어 구성된다. 서비스 환경은 추후 서비스를 위해 설치된 환경이고, 개발환경은 개발의 편의를 위해 임시로 설치된 테스트 머신 이다. 현재 개발환경, 서비스 환경 모두 아래 사항으로 모든 프로그램들이 설치되어는 있지만 본 프로젝트는 개발환경에만 올라가 있는 상태이다. 추후 서비스 환경으로 변경할 예정이다. 또한, 초기 제안서에 누락된 Apache와 Tomcat의 Connection 역할을 해주는 Jakarta가 있다.

### 개발 환경 ###
|운영체제|Windows XP|
|:-----------|:---------|
|WebServer|Tomcat 5.5, Apache 2.0, Jakarta 1.2.15|
|RDBMS|Mysql 5.0|
|Language|Java(JSP)|
|Library|Mysql JDBC 3.1, JDK 1\_5\_0|
|사용프로그램|Eclipse|

# 프로젝트 기능 및 구현 #
## Client ##
### 구성 ###
Client는 Internet Explorer의 Toolbar 형태와 BHO(Browser Helper Object)로 등록되게 된다. 아래는 Toolbar와 BHO로 등록하기 위한 레지스트리 정보이다. 본 프로젝트에 MyFirstMateBarBand.rgs에 포함되어있고 레지스트리 등록 시 실행된다.

### BHO(Browser Helper Object) 란? ###
BHO의 공식 명칭은 사실 Internet Explorer add-ins using the COM technology이다. BHO는 COM(최소한의 COM 기능을 갖는다.)의 In-Process 모델에 IObjectWithSite 인터페이스 추가와 Explorer Extension으로 등록한 Object를 말한다.
```
HKLM {
NoRemove Software {
NoRemove Microsoft {
NoRemove Windows {
NoRemove CurrentVersion {
NoRemove Explorer {
NoRemove 'Browser Helper Objects' {
NoRemove '{F1518563-F371-4c52-B70E-DE39A14265CE}'
}
}
}
}
}
}
}

HKLM {
NoRemove Software {
NoRemove Microsoft {
NoRemove 'Internet Explorer' {
NoRemove Toolbar {
val '{F1518563-F371-4c52-B70E-DE39A14265CE}' = s 'MyFirstMateBar'
}
}
}
}
}
```

Toolbar와 BHO로 등록되었는지 여부는 레지스트리를 확인함으로도 가능하지만 Internet Explorer 메뉴 중 [도구] -> [추가 기능 관리]를 통해서 다음과 같이 확인도 가능하다.


### 기능 ###
Client의 가장 핵심이 되는 기능으로 웹 페이지 스크랩 기능을 들 수 있다. 이 기능은 웹 페이지를 선택한 상태에서 마우스 오른쪽 버튼을 클릭함으로써 사용할 수 있다. 아래 그림은 해당 영역을 선택하고 마우스 오른쪽 버튼을 클릭하며 Post It 기능을 사용하려는 화면을 보여주고 있다. 실제 이 부분은 JavaScript로 이루어져 웹 페이지에서의 선택한 영역의 html을 JavaScript를 이용하여 Client 프로그램에 전달하게 된다. Client 프로그램은 그 데이터를 받아 '클립보드 복사', '파일로 저장', '서버에 Post 등록' 등 전달받은 데이터를 가공하는 작업을 하게 된다.

아래는 JavaScript를 이용하여 Client로 데이터를 전송하기 위한 레지스트리 정보와 JavaScript의 내용이다.

```
HKCU {
NoRemove Software {
NoRemove Microsoft {
NoRemove 'Internet Explorer' {
NoRemove MenuExt {
ForceRemove 'MFM Post It(&M)' = s 
'res://%MODULE%/savepost.html' {
val Contexts = b '10'
}
}
}
}
}
}
```

MenuExt에 등록된 부분이 마우스 오른쪽 버튼을 클릭했을 때 나타나는 메뉴가 되고 Contexts의 값에 따라 여러 가지 상황이 제한되게 된다. 여기서 ‘res://xxx’을 이용하여 Client의 리소스에 포함된 JavaScript를 실행할 수 있게 된다. 아래는 savepost.html의 내용이다.
```
<object	id=MFMControl align=top  classid="clsid:F1518563-F371-4c52-B70E-DE39A14265CE" width=1 height=1 border=0 vspace=0></object>
<SCRIPT LANGUAGE="JavaScript">
	var win = external.menuArguments;
	var doc = win.document;
	var sel = doc.selection;
	var rng = sel.createRange();
	var str = new String(rng.htmlText);
	var href = doc.URLUnencoded;

	var objMFM = document.getElementById("MFMControl");
	objMFM.SetSavePost(href, str);
</SCRIPT>
```
위 기능을 이용하여 서버에 저장할 수 있는 기능은 물론이고 저장 시 제목과 본문을 원하는 형식으로 수정하는 기능, 그리고 선택한 본문을 클립보드로 복사하는 기능, 파일로 저장하는 기능을 제공한다. 아래는 파일로 저장된 콘텐츠의 예를 보여주고 있다.

### 구조 ###
아래 그림은 주요 클래스와 그 클래스 내부에 주요 메서드, 속성들의 클래스 다이어그램을 나타낸다. [그림 12]는 CMyFirstMateBarBand의 클래스 구조와 그 의 멤버인 IInputObjectSitePtr형 객체인 m\_pIOSite의 관계를 나타내고 있다. IObjectWithSite::SetSite 메서드는 IUnknown\*형의 파라미터를 갖고 있고 이 파라 미터를 통해 Web Browser object를 접근할 수 있는 권한을 갖게 되는 구조이다.