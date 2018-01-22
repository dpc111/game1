-----------------------------------------------------
-- do not modify the file, gen by const/gen.bat
-----------------------------------------------------
--
tip = {}
tip.table_not_exist           = 1               -- 牌桌存在
--
join_table = {}
join_table.not_empty_seat     = 2               -- 没有空座位
--
op = {}
op.error_op                   = 3               -- 错误操作
op.player_not_on_table        = 4               -- 玩家不在牌桌上
op.not_turn_you               = 5               -- 没有轮到你操作
op.bet_not_enough             = 6               -- 下注不够
op.can_not_check              = 7               -- 不能过牌
op.can_not_op                 = 8               -- 当前不能操作
