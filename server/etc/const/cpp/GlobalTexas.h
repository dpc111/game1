/////////////////////////////////////////////////////
// do not modify the file, gen by const/gen.bat
/////////////////////////////////////////////////////
//牌桌状态
#define table_state_wait                1               // 等待
#define table_state_begin               2               // 开始
#define table_state_deal_hand_card      3               // 发手牌
#define table_state_deal_table_card     4               // 发公共牌
#define table_state_bet                 5               // 下注
#define table_state_gen_pool            6               // 生成边池
#define table_state_award               7               // 奖励
//玩家状态
#define player_state_prepare            1               // 准备
#define player_state_discard            2               // 弃牌
#define player_state_all_in             3               // 全下
#define player_state_normal             4               // 普通
