-----------------------------------------------------
-- do not modify the file, gen by const/gen.bat
-----------------------------------------------------
--����
MatchFormat = {}
MatchFormat.Fixed                    = 1               -- ���ֻ�����
MatchFormat.Out                      = 2               -- ���������Ʊ�
MatchFormat.Ass                      = 3               -- ASS
MatchFormat.Swit                     = 6               -- ��ʿ��λ��
--��ʱ
MatchDelayTime = {}
MatchDelayTime.Begin                 = 1               -- ""
MatchDelayTime.TableReleaseCheck     = 2               -- ""
--��̭��������
MatchOutUp = {}
MatchOutUp.Time                      = 1               -- ��ʱ��
MatchOutUp.Inning                    = 2               -- ����
--������������
MatchOpen = {}
MatchOpen.PlayerNum                  = 1               -- ����
MatchOpen.Time                       = 2               -- ʱ��
--�������״̬
PlayerMatchState = {}
PlayerMatchState.Wait                = 1               -- �ȴ�
PlayerMatchState.Playing             = 2               -- ����
--����״̬
MatchState = {}
MatchState.Apply                     = 1               -- ����
MatchState.Playing                   = 2               -- ����
MatchState.Over                      = 3               -- ����
--����
MatchStage = {}
MatchStage.Begin                     = 0               -- ��ʼֵ
MatchStage.Stage1                    = 1               -- ""
MatchStage.Stage2                    = 2               -- ""
MatchStage.Stage3                    = 3               -- ""
MatchStage.End                       = 4               -- ����ֵ
--��̭����
WeedOutType = {}
WeedOutType.Percent                  = 1               -- �ٷֱ�
WeedOutType.Num                      = 2               -- ��ֵ
