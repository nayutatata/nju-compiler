#include "ir.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define TODO assert(0)
static void print_entry(code_entry* ce,FILE* file) {
    operator_kind opk = ce->op.kind;
    if (opk==OP_LABELDEC){
        fprintf(file, "LABEL %s :", ce->arg1.name);
    }
    else if (opk==OP_FUNCDEC){
        fprintf(file, "FUNCTION %s :", ce->arg1.name);
    }
    else if (opk==OP_ASSIGN){// in fact arg1 equals to result
        fprintf(file, "%s := %s", ce->arg1.name, ce->arg2.name);
    }
    else if (opk==OP_PLUS){
        fprintf(file, "%s := %s + %s", ce->result.name, ce->arg1.name, ce->arg2.name);
    }
    else if (opk==OP_MINUS){
        fprintf(file, "%s := %s - %s", ce->result.name, ce->arg1.name, ce->arg2.name);
    }
    else if (opk==OP_MUL){
        fprintf(file, "%s := %s * %s", ce->result.name, ce->arg1.name, ce->arg2.name);
    }
    else if (opk==OP_DIV){
        fprintf(file, "%s := %s / %s", ce->result.name, ce->arg1.name, ce->arg2.name);
    }
    else if (opk==OP_ADDR){
        fprintf(file, "%s := &%s", ce->result.name, ce->arg1.name);
    }
    else if (opk==OP_STAR){
        fprintf(file, "%s := *%s", ce->result.name, ce->arg1.name);
    }
    else if (opk==OP_TOSTAR){
        fprintf(file, "*%s := %s", ce->result.name, ce->arg1.name);
    }
    else if (opk==OP_GOTO){
        fprintf(file, "GOTO %s", ce->result.name);
    }
    else if (opk==OP_IF_GOTO){
        fprintf(file, "IF %s %s %s GOTO %s", ce->arg1.name, ce->gotoop.name,ce->arg2.name,ce->result.name);
    } 
    else if (opk == OP_RETURN) {
        fprintf(file, "RETURN %s", ce->arg1.name);
    } else if (opk == OP_DEC) {
        fprintf(file, "DEC %s %s", ce->arg1.name, ce->arg2.name);
    } else if (opk == OP_ARG) {
        fprintf(file, "ARG %s", ce->arg1.name);
    } else if (opk == OP_CALL) {
        fprintf(file, "%s := CALL %s", ce->result.name, ce->arg1.name);
    } else if (opk == OP_PARAM) {
        fprintf(file, "PARAM %s", ce->arg1.name);
    } else if (opk == OP_READ) {
        fprintf(file, "READ %s", ce->result.name);
    } else if (opk == OP_WRITE) {
        fprintf(file, "WRITE %s", ce->arg1.name);
    }
    else
        fprintf(file, "No such op_kind.");
}
static char* relop_table[] = {
    ">=", ">", "<=", "<", "==", "!="};
void init_code_list() {
    clst.num = 0;
}
void add_code(code_entry* ce){
    clst.lst[clst.num++] = ce;
}
code_entry* new_ce(){
    code_entry* res = malloc(sizeof(code_entry));
    res->arg1.kind = res->arg2.kind = res->result.kind = OPE_NULL;
    res->op.kind = OP_NULL;
    return res;
}
operand_t* new_temp(){
    static int tindex = 0;
    operand_t* res = malloc(sizeof(operand_t));
    res->kind = OPE_TEMP;
    sprintf(res->name, "t%d", tindex++);
    return res;
}
operand_t* new_label(){
    static int lindex = 0;
    operand_t* res = malloc(sizeof(operand_t));
    res->kind = OPE_LABEL;
    sprintf(res->name, "label%d", lindex++);
    return res;
}
operand_t* new_imm(int val){
    operand_t* res = malloc(sizeof(operand_t));
    res->kind = OPE_IMM;
    sprintf(res->name, "#%d", val);
    res->imm_v = val;
    return res;
}
void print_code_list(FILE* file){
    //printf("total:%d\n", clst.num);
    //printf("===================\n");
    if (file == NULL)
        file = stdout;
    for (int i = 0; i < clst.num;i++){
        print_entry(clst.lst[i],file);
        fprintf(file, "\n");
    }
}
void translate(Node* root){
    tr_program(root);
}
void tr_program(Node* root){
    Node* extdeflist = getchild(root, 0);
    tr_extdeflist(extdeflist);
}
void tr_extdeflist(Node* root){
    if (!root)
        return;
    Node* extdef = getchild(root, 0);
    Node* extdeflist = getchild(root, 1);
    tr_extdef(extdef);
    tr_extdeflist(extdeflist);
}
void tr_extdef(Node* root){
    Node* specifier = getchild(root, 0);
    Node* fundec = getchild(root, 1);
    if (root->ccnt == 3&&fundec->ntype==_FunDec) {
        Node* compst = getchild(root, 2);
        //tr_specifier(specifier);
        tr_funcdec(fundec);
        tr_compst(compst);
        return;
    }
    exit(0);
}
operand_t* tr_vardec(Node* root,int size){
    if (root->ccnt==1){
        Node* id = getchild(root, 0);
        sym_entry* se = find_sym_entry(id->val.name);
        assert(se);
        if (se->type->kind==SYM_STRUCT){
            exit(0);
        }
        operand_t* res = malloc(sizeof(operand_t));
        res->kind = OPE_VAR;
        strcpy(res->name, se->name);
        if (se->type->kind==SYM_BASIC){
            return res;
        }
        if (se->type->kind==SYM_ARRAY){
            code_entry* ce = new_ce();
            ce->op.kind = OP_DEC;
            ce->arg1.kind = OPE_VAR;
            ce->arg2.kind = OPE_IMM;
            strcpy(ce->arg1.name, se->name);
            sprintf(ce->arg2.name, "%d", size*4);
            ce->arg2.imm_v = size * 4;
            add_code(ce);
            return res;
        }
        assert(0);
    }
    Node* vardec = getchild(root, 0);
    Node* i = getchild(root, 2);
    int z = i->val.intv;
    return tr_vardec(vardec, size*z);
}
void tr_funcdec(Node* root){
    Node* id = getchild(root, 0);
    sym_entry* se = find_sym_entry(id->val.name);

    code_entry* fundec = new_ce();
    fundec->arg1.kind = OPE_FUNC;
    strcpy(fundec->arg1.name, id->val.name);
    fundec->op.kind = OP_FUNCDEC;
    add_code(fundec);

    field_node* fn = se->type->func_info.param_types;
    while (fn){
        sym_entry* temp = find_sym_entry(fn->name);
        assert(temp);
        if (temp->type->kind==SYM_ARRAY)
            temp->is_array_param = 1;

        code_entry* param = new_ce();
        param->op.kind = OP_PARAM;
        param->arg1.kind = OPE_VAR;
        strcpy(param->arg1.name, fn->name);
        add_code(param);
        fn = fn->next;
    }
}
void tr_compst(Node* root){
    Node* deflist = getchild(root, 1);
    Node* stmtlist = getchild(root, 2);
    tr_deflist(deflist);
    tr_stmtlist(stmtlist);
}
void tr_stmtlist(Node* root){
    if (!root)
        return;
    Node* stmt = getchild(root,0);
    Node* stmtlist = getchild(root, 1);
    tr_stmt(stmt);
    tr_stmtlist(stmtlist);
}
void tr_stmt(Node* root){
    int cnt = root->ccnt;
    if (cnt==1){
        Node* compst = getchild(root,0);
        tr_compst(compst);
    }
    else if (cnt==2){//exp
        Node* exp = getchild(root, 0);
        tr_exp(exp);
    }
    else if (cnt==3){ //return
        Node* exp = getchild(root, 1);
        code_entry* ce_exp = tr_exp(exp);
        code_entry* res = new_ce();
        res->op.kind = OP_RETURN;
        res->arg1 = to_value(ce_exp->result);
        add_code(res);
    }
    else{
        Node* c0 = getchild(root, 0);
        if (c0->ntype==_IF){
            Node *exp = getchild(root, 2), *stmt1 = getchild(root, 4);
            operand_t *label1 = new_label(), *label2 = new_label();
            tr_cond(exp, *label1, *label2);
            code_entry* res1 = new_ce();
            res1->op.kind = OP_LABELDEC;
            res1->arg1 = *label1;
            add_code(res1);
            tr_stmt(stmt1);
            code_entry* res2 = new_ce();
            res2->op.kind = OP_LABELDEC;
            res2->arg1 = *label2;
            if (cnt==5){
                add_code(res2);
                return;
            }
            Node* stmt2 = getchild(root, 6);
            operand_t* label3 = new_label();
            code_entry* res3 = new_ce();
            res3->op.kind = OP_GOTO;
            res3->result = *label3;
            add_code(res3);
            add_code(res2);
            tr_stmt(stmt2);
            code_entry* res4 = new_ce();
            res4->op.kind = OP_LABELDEC;
            res4->arg1 = *label3;
            add_code(res4);
        } else {  // while
            Node* exp = getchild(root, 2);
            Node* stmt = getchild(root, 4);
            operand_t *label1 = new_label(), *label2 = new_label(), *label3 = new_label();
            code_entry *res1 = new_ce(), *res2 = new_ce(), *res3 = new_ce(), *res4 = new_ce();
            res1->op.kind = res2->op.kind = res4->op.kind = OP_LABELDEC;
            res1->arg1 = *label1, res2->arg1 = *label2, res4->arg1 = *label3;
            res3->op.kind = OP_GOTO;
            res3->result = *label1;
            add_code(res1);
            tr_cond(exp, *label2, *label3);
            add_code(res2);
            tr_stmt(stmt);
            add_code(res3);
            add_code(res4);
            return;
        }
    }
}
void tr_deflist(Node* root){
    if (!root)
        return;
    Node* def = getchild(root, 0);
    Node* deflist = getchild(root, 1);
    tr_def(def);
    tr_deflist(deflist);
}
void tr_def(Node* root){
    Node* specifier = getchild(root, 0);
    Node* declist = getchild(root, 1);
    tr_declist(declist);
}
void tr_declist(Node* root){
    Node* dec = getchild(root, 0);
    tr_dec(dec);
    if (root->ccnt==1)
        return;
    Node* declist = getchild(root, 2);
    tr_declist(declist);
}
void tr_dec(Node* root){
    Node* vardec = getchild(root, 0);
    operand_t* ope = tr_vardec(vardec,1);
    if (root->ccnt==1)
        return;
    Node* exp = getchild(root, 2);
    code_entry* ce_exp = tr_exp(exp);
    code_entry* ce = process_assign(*ope, ce_exp->result);
}
code_entry* tr_exp(Node* root){
    code_entry* res = new_ce();
    if (root->ccnt == 1) {
        Node* child = getchild(root, 0);
        if (child->ntype==_ID){
            res->result.kind = OPE_VAR;
            sym_entry* se = find_sym_entry(child->val.name);
            assert(se);
            strcpy(res->result.name, se->name);
        }
        else if (child->ntype==_INT){
            res->result.kind = OPE_IMM;
            sprintf(res->result.name, "#%d", child->val.intv);
            res->result.imm_v = child->val.intv;
        } else if (child->ntype == _FLOAT) {
            res->result.kind = OPE_IMM;
            sprintf(res->result.name, "#%f", child->val.floatv);
            res->result.imm_fv = child->val.floatv;
        }
        return res;
    } else if (root->ccnt == 2) {  // NOT,MINUS
        Node* op = getchild(root, 0);
        Node* exp = getchild(root, 1);
        operand_t* exp_res = new_temp();
        code_entry* ce_exp = tr_exp(exp);
        if (op->ntype==_MINUS){
            res->op.kind = OP_MINUS;
            res->arg1.kind = OPE_IMM;
            sprintf(res->arg1.name, "#0");
            res->arg2 = to_value(ce_exp->result);
            res->result = *exp_res;
            add_code(res);
        } else if (op->ntype == _NOT) {
            operand_t *label1 = new_label(), *label2 = new_label();
            code_entry* res1 = new_ce();
            res1->op.kind = OP_ASSIGN;
            res1->arg1 = res->result;
            res1->arg2 = *(new_imm(0));
            res1->result = res->result;
            add_code(res1);

            tr_cond(root, *label1, *label2);

            code_entry* res2 = new_ce();
            res2->op.kind = OP_LABELDEC;
            res2->arg1 = *label1;
            add_code(res2);

            code_entry* res3 = new_ce();
            res3->op.kind = OP_ASSIGN;
            res3->arg1 = res->result;
            res3->arg2 = *(new_imm(1));
            res3->result = res->result;
            add_code(res3);

            code_entry* res4 = new_ce();
            res4->op.kind = OP_LABELDEC;
            res4->arg1 = *label2;
            add_code(res4);
        }
    } else if (root->ccnt == 3) {
        Node *c0 = getchild(root, 0), *c1 = getchild(root, 1), *c2 = getchild(root, 2);
        if (c0->ntype==_LP){// (exp)
            return  tr_exp(c1);
        } else if (c1->ntype == _LP) {  // f()
            operand_t* t = new_temp();
            sym_entry* se = find_sym_entry(c0->val.name);
            assert(se);
            if (strcmp(se->name,"read")!=0){
                res->op.kind = OP_CALL;
                res->arg1.kind = OPE_FUNC;
                strcpy(res->arg1.name, se->name);
                res->result = *t;
            }
            else{
                res->op.kind = OP_READ;
                res->result = *t;
            }
            add_code(res);
        } else if (c1->ntype == _DOT) {
            assert(0); // should not appear
        }
        // following is 2-op exp
        else {
            operand_t* t = new_temp();
            code_entry *ce0 = tr_exp(c0), *ce2 = tr_exp(c2);
            res->arg1 = ce0->result;
            res->arg2 = ce2->result;
            res->result = *t;
            switch (c1->ntype)
            {
            case _ASSIGNOP:
                res = process_assign(res->arg1, res->arg2);
                return res;
            case _AND:
            case _OR:
            case _RELOP:{
                operand_t *label1 = new_label(), *label2 = new_label();
                code_entry* res1 = new_ce();
                res1->op.kind = OP_ASSIGN;
                res1->arg1 = res->result;
                res1->arg2 = *(new_imm(0));
                res1->result = res->result;
                add_code(res1);

                tr_cond(root, *label1, *label2);

                code_entry* res2 = new_ce();
                res2->op.kind = OP_LABELDEC;
                res2->arg1 = *label1;
                add_code(res2);

                code_entry* res3 = new_ce();
                res3->op.kind = OP_ASSIGN;
                res3->arg1 = res->result;
                res3->arg2 = *(new_imm(1));
                res3->result = res->result;
                add_code(res3);

                code_entry* res4 = new_ce();
                res4->op.kind = OP_LABELDEC;
                res4->arg1 = *label2;
                add_code(res4);
                break;
            }
            case _PLUS:
                res->op.kind = OP_PLUS;
                res->arg1 = to_value(res->arg1);
                res->arg2 = to_value(res->arg2);
                add_code(res);
                break;
            case _MINUS:
                res->op.kind = OP_MINUS;
                res->arg1 = to_value(res->arg1);
                res->arg2 = to_value(res->arg2);
                add_code(res);
                break;
            case _STAR:
                res->op.kind = OP_MUL;
                res->arg1 = to_value(res->arg1);
                res->arg2 = to_value(res->arg2);
                add_code(res);
                break;
            case _DIV:
                res->op.kind = OP_DIV;
                res->arg1 = to_value(res->arg1);
                res->arg2 = to_value(res->arg2);
                add_code(res);
                break;
            default:
                assert(0);
                break;
            }
        }
    } else if (root->ccnt == 4) {
        Node *c0 = getchild(root, 0), *c2 = getchild(root, 2);
        if (c0->ntype==_ID){// f(a,b,c)
            sym_entry* se = find_sym_entry(c0->val.name);
            assert(se);
            operand_t* t = new_temp();
            if (strcmp(se->name,"write")!=0){
                res->op.kind = OP_CALL;
                res->arg1.kind = OPE_FUNC;
                strcpy(res->arg1.name, se->name);
                res->result = *t;
                tr_args(c2);
            }
            else{
                res->op.kind = OP_WRITE;
                Node* exp = getchild(c2, 0);
                code_entry* ce = tr_exp(exp);
                res->arg1 = to_value(ce->result);
                res->result = *t;
            }
            add_code(res);
        } else if (c0->ntype == _Exp) {  // array
            // only return a address
            operand_t addr = get_addr(root,0);
            res->op.kind = OP_NULL;
            res->result = addr;
            res->result.is_arr = 1; // is an array address.
        }
    }
    return res;
}
void tr_args(Node* root){
    if (root->ccnt==1){
        Node* exp = getchild(root, 0);
        code_entry* ce_exp = tr_exp(exp);
        sym_entry* se = find_sym_entry(ce_exp->result.name);
        code_entry* res = new_ce();
        if (!se || se->type->kind != SYM_ARRAY) {
            res->op.kind = OP_ARG;
            res->arg1 = to_value(ce_exp->result);
            add_code(res);
        } else { // array name
            if (!se->is_array_param) {
                code_entry* t = new_ce();
                t->op.kind = OP_ADDR;
                t->arg1 = ce_exp->result;
                t->result = *(new_temp());
                add_code(t);

                res->op.kind = OP_ARG;
                res->arg1 = t->result;
                t->result.is_arg = 1;
                add_code(res);
            }
            else{
                res->op.kind = OP_ARG;
                res->arg1 = ce_exp->result;
                res->arg1.is_arg = 1;
                add_code(res);
            }
        }
    }
    else{
        Node* args = getchild(root, 2);
        tr_args(args);
        Node* exp = getchild(root, 0);
        code_entry* ce_exp = tr_exp(exp);
        sym_entry* se = find_sym_entry(ce_exp->result.name);
        code_entry* res = new_ce();
        if (!se || se->type->kind != SYM_ARRAY) {
            res->op.kind = OP_ARG;
            res->arg1 = to_value(ce_exp->result);
            add_code(res);
        } else {
            if (!se->is_array_param) {
                code_entry* t = new_ce();
                t->op.kind = OP_ADDR;
                t->arg1 = ce_exp->result;
                t->result = *(new_temp());
                add_code(t);

                res->op.kind = OP_ARG;
                res->arg1 = t->result;
                t->result.is_arg = 1;
                add_code(res);
            } else {
                res->op.kind = OP_ARG;
                res->arg1 = ce_exp->result;
                res->arg1.is_arg = 1;
                add_code(res);
            }
        }
    }
}
void tr_cond(Node* root,operand_t true_label,operand_t false_label){
    int cnt = root->ccnt;
    if (cnt==2){ // NOT
        Node* not = getchild(root, 0), *exp = getchild(root, 1);
        if (not->ntype==_NOT&&exp->ntype==_Exp){
            tr_cond(exp, false_label, true_label);
            return;
        }
    }
    if (cnt==3){
        Node *exp1 = getchild(root, 0), *op = getchild(root, 1), *exp2 = getchild(root, 2);
        if (op->ntype==_AND){
            operand_t* label1 = new_label();
            code_entry* res = new_ce();
            res->op.kind = OP_LABELDEC;
            res->arg1 = *label1;
            tr_cond(exp1, *label1, false_label);
            add_code(res);
            tr_cond(exp2, true_label, false_label);
            return;
        }
        else if (op->ntype==_OR){
            operand_t* label1 = new_label();
            code_entry* res = new_ce();
            res->op.kind = OP_LABELDEC;
            res->arg1 = *label1;
            tr_cond(exp1, true_label, *label1);
            add_code(res);
            tr_cond(exp2, true_label, false_label);
            return;
        }
        else if (op->ntype==_RELOP){
            code_entry *ce1 = tr_exp(exp1), *ce2 = tr_exp(exp2);
            code_entry* res = new_ce();
            res->op.kind = OP_IF_GOTO;
            res->arg1 = to_value(ce1->result);
            res->arg2 = to_value(ce2->result);
            // res->op.name
            strcpy(res->gotoop.name, relop_table[op->val.opv]);
            res->result = true_label;
            add_code(res);
            code_entry* res2 = new_ce();
            res2->op.kind = OP_GOTO;
            res2->result = false_label;
            add_code(res2);
            return;
        }
    }
    code_entry* ce_exp = tr_exp(root);
    code_entry* res = new_ce();
    res->op.kind = OP_IF_GOTO;
    // res->op.name
    strcpy(res->gotoop.name, relop_table[NE]);
    res->arg1 = to_value(ce_exp->result);
    res->arg2 = *(new_imm(0));
    res->result = true_label;
    add_code(res);
    code_entry* res2 = new_ce();
    res2->op.kind = OP_GOTO;
    res2->result = false_label;
    add_code(res2);
}
operand_t get_addr(Node* root,int dim){
    static operand_t offlist[40];
    int cnt = root->ccnt;
    if (cnt==1){
        Node* id = getchild(root, 0);
        sym_entry* se = find_sym_entry(id->val.name);

        operand_t offset = *(new_temp());
        code_entry* t = new_ce();
        t->op.kind = OP_ASSIGN;
        t->result = offset;
        t->arg1 = offset;
        t->arg2 = *(new_imm(0));
        add_code(t);

        operand_t base = *(new_temp());

        t = new_ce();
        t->op.kind = OP_ASSIGN;
        t->result = base;
        t->arg1 = base;
        t->arg2 = *(new_imm(4));
        add_code(t);
        // something wrong. base cannot be evaluated by pre-order.
        for (int i = 0; i < dim; i++) {
            operand_t a = offlist[i];
            int b = se->type->array_info.size[dim - i - 1];
            code_entry* temp = new_ce();
            temp->op.kind = OP_MUL;
            temp->arg1 = a;
            temp->arg2 = base;
            temp->result = *(new_temp());
            add_code(temp);

            code_entry* temp1 = new_ce();
            temp1->op.kind = OP_PLUS;
            temp1->arg1 = offset;
            temp1->arg2 = temp->result;
            temp1->result = offset;
            add_code(temp1);

            code_entry* temp2 = new_ce();
            temp2->op.kind = OP_MUL;
            temp2->arg1 = base;
            temp2->arg2 = *(new_imm(b));
            temp2->result = base;
            add_code(temp2);
        }
        // something wrong. when array is arguement, should not addr it.
        operand_t temp;
        strcpy(temp.name, se->name);
        temp.kind = OPE_VAR;
        if (!se->is_array_param) {
            code_entry* res1 = new_ce();
            res1->op.kind = OP_ADDR;
            res1->arg1.kind = OPE_VAR;
            res1->result = *(new_temp());
            temp = res1->result;
            strcpy(res1->arg1.name, se->name);
            add_code(res1);
        }

        code_entry* res2 = new_ce();
        res2->op.kind = OP_PLUS;
        res2->arg1 = temp;
        res2->arg2 = offset;
        res2->result = *(new_temp());
        add_code(res2);
        if (se->type->array_info.num != dim) {
            res2->result.should_be_array = 1;
        }
        return res2->result;
    }
    /*
    exp
    exp [1]
    exp [2] [1]
    id [2] [1]
    */
    Node *exp1 = getchild(root, 0), *exp2 = getchild(root, 2);
    code_entry* ce_exp2 = tr_exp(exp2);
    offlist[dim] = ce_exp2->result;
    return get_addr(exp1, dim + 1);
}
code_entry* process_assign(operand_t a,operand_t b){
    if (a.is_arr&&b.is_arr){
        code_entry* res1 = new_ce();
        res1->op.kind = OP_STAR;
        res1->arg1 = b;
        res1->result = *(new_temp());
        add_code(res1);

        code_entry* res2 = new_ce();
        res2->op.kind = OP_TOSTAR;
        res2->arg1 = res1->result;
        res2->result = a;
        add_code(res2);
        return res2;
    }
    if (a.is_arr&&!b.is_arr){
        code_entry* res = new_ce();
        res->op.kind = OP_TOSTAR;
        res->arg1 = b;
        res->result = a;
        add_code(res);
        return res;
    }
    if (!a.is_arr&&b.is_arr){
        code_entry* res = new_ce();
        res->op.kind = OP_STAR;
        res->arg1 = b;
        res->result = a;
        add_code(res);
        return res;
    }
    sym_entry* se1 = find_sym_entry(a.name);
    sym_entry* se2 = find_sym_entry(b.name);
    if (se1!=NULL&&se2!=NULL&&se1->type->kind==SYM_ARRAY&&se2->type->kind==SYM_ARRAY){
        TODO;
    }
    code_entry* res = new_ce();
    res->op.kind = OP_ASSIGN;
    res->arg1 = a;
    res->arg2 = b;
    res->result = a;
    add_code(res);
    return res;
}
operand_t to_value(operand_t ope){
    if (ope.should_be_array){
        return ope;
    }
    if (ope.is_arr){
        code_entry* ce = new_ce();
        ce->op.kind = OP_STAR;
        ce->arg1 = ope;
        ce->result = *(new_temp());
        add_code(ce);
        return ce->result;
    }
    return ope;
}