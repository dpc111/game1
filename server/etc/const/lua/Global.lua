-----------------------------------------------------
-- do not modify the file, gen by const/gen.bat
-----------------------------------------------------
--赛制
MatchFormat = {}
MatchFormat.Fixed                    = 1               -- 定局积分制
MatchFormat.Out                      = 2               -- 打立出局制表
MatchFormat.Ass                      = 3               -- ASS
MatchFormat.Swit                     = 6               -- 瑞士移位法
--延时
MatchDelayTime = {}
MatchDelayTime.Begin                 = 1               -- ""
MatchDelayTime.TableReleaseCheck     = 2               -- ""
--淘汰积分增长
MatchOutUp = {}
MatchOutUp.Time                      = 1               -- 按时间
MatchOutUp.Inning                    = 2               -- 按局
--开赛条件类型
MatchOpen = {}
MatchOpen.PlayerNum                  = 1               -- 人数
MatchOpen.Time                       = 2               -- 时间
--比赛玩家状态
PlayerMatchState = {}
PlayerMatchState.Wait                = 1               -- 等待
PlayerMatchState.Playing             = 2               -- 在玩
--比赛状态
MatchState = {}
MatchState.Apply                     = 1               -- 报名
MatchState.Playing                   = 2               -- 在玩
MatchState.Over                      = 3               -- 结束
--赛程
MatchStage = {}
MatchStage.Begin                     = 0               -- 开始值
MatchStage.Stage1                    = 1               -- ""
MatchStage.Stage2                    = 2               -- ""
MatchStage.Stage3                    = 3               -- ""
MatchStage.End                       = 4               -- 结束值
--淘汰类型
WeedOutType = {}
WeedOutType.Percent                  = 1               -- 百分比
WeedOutType.Num                      = 2               -- 数值
