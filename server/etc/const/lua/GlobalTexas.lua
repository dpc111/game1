-----------------------------------------------------
-- do not modify the file, gen by const/gen.bat
-----------------------------------------------------
--牌桌状态
table_state = {}
table_state.wait                = 1               -- 等待
table_state.begin               = 2               -- 开始
table_state.deal_hand_card      = 3               -- 发手牌
table_state.deal_table_card     = 4               -- 发公共牌
table_state.bet                 = 5               -- 下注
table_state.gen_pool            = 6               -- 生成边池
table_state.award               = 7               -- 奖励
--玩家状态
player_state = {}
player_state.prepare            = 1               -- 准备
player_state.discard            = 2               -- 弃牌
player_state.all_in             = 3               -- 全下
player_state.normal             = 4               -- 普通
