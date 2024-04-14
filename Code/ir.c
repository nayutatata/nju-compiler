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
        fprintf(file, "DEC %s [%s]", ce->arg1.name, ce->arg2.name);
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
    assert(0);
}
operand_t* tr_vardec(Node* root,int size){
    if (root->ccnt==1){
        Node* id = getchild(root, 0);
        sym_entry* se = find_sym_entry(id->val.name);
        assert(se);
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
            sprintf(ce->arg2.name, "#%d", size);
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
        res->arg1 = ce_exp->result;
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
    code_entry* ce = new_ce();
    ce->op.kind = OP_ASSIGN;
    ce->result = ce->arg1 = *ope;
    ce->arg2 = ce_exp->result;
    add_code(ce);
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
        }
        else if (child->ntype==_FLOAT){
            res->result.kind = OPE_IMM;
            sprintf(res->result.name, "#%f", child->val.floatv);
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
            res->arg2 = ce_exp->result;
            res->result = *exp_res;
        }
        else if (op->ntype==_NOT){
            
        }
    } else if (root->ccnt == 3) {
        Node *c0 = getchild(root, 0), *c1 = getchild(root, 1), *c2 = getchild(root, 2);
        if (c0->ntype==_LP){// (exp)
            return tr_exp(c1);
        }
        else if (c1->ntype==_LP){// f()
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
        }
        else if(c1->ntype==_DOT){
            assert(0); // should not appear
        }
        // following is 2-op exp
        else{
            operand_t* t = new_temp();
            code_entry *ce0 = tr_exp(c0), *ce2 = tr_exp(c2);
            res->arg1 = ce0->result;
            res->arg2 = ce2->result;
            res->result = *t;
            switch (c1->ntype)
            {
            case _ASSIGNOP:
                res->op.kind = OP_ASSIGN;
                break;
            case _AND:
                TODO;
                break;
            case _OR:
                TODO;
                break;
            case _RELOP:
                TODO;
                break;
            case _PLUS:
                res->op.kind = OP_PLUS;
                break;
            case _MINUS:
                res->op.kind = OP_MINUS;
                break;
            case _STAR:
                res->op.kind = OP_MUL;
                break;
            case _DIV:
                res->op.kind = OP_DIV;
                break;
            case _NOT:
                TODO;
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
                res->arg1 = ce->result;
                res->result = *t;
            }
        }
        else if (c0->ntype==_Exp){ //array
            // one-dimension

            res->op.kind = OP_STAR;
            res->result = *(new_temp());
            res->arg1 = final_addr->result;
        }
    }
    add_code(res);
    return res;
}
void tr_args(Node* root){
    Node* exp = getchild(root,0);
    code_entry* ce_exp = tr_exp(exp);
    code_entry* res = new_ce();
    res->op.kind = OP_ARG;
    res->arg1 = ce_exp->result;
    add_code(res);
    if (root->ccnt==1)
        return;
    Node* args = getchild(root, 2);
    tr_args(args);
}
void tr_cond(Node* root,operand_t true_label,operand_t false_label){
    int cnt = root->ccnt;
    if (cnt==2){
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
            res->arg1 = ce1->result;
            res->arg2 = ce2->result;
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
    res->arg1 = ce_exp->result;
    res->arg2 = *(new_imm(0));
    res->result = true_label;
    add_code(res);
    code_entry* res2 = new_ce();
    res2->op.kind = OP_GOTO;
    res2->result = false_label;
    add_code(res2);
}
code_entry* get_addr(Node* root){
    Node* array = getchild(root, 0);
    Node* id = getchild(array, 0);
    Node* exp = getchild(root, 2);
    sym_entry* se = find_sym_entry(id->val.name);

    code_entry* exp_ce = tr_exp(exp);  // offset

    code_entry* addr = new_ce();
    addr->op.kind = OP_ADDR;
    addr->arg1.kind = OPE_VAR;
    strcpy(addr->arg1.name, se->name);
    addr->result = *(new_temp());
    add_code(addr);

    code_entry* elem_addr = new_ce();
    elem_addr->op.kind = OP_MUL;
    elem_addr->arg1 = *(new_imm(4));
    elem_addr->arg2 = exp_ce->result;
    elem_addr->result = *(new_temp());
    add_code(elem_addr);

    code_entry* final_addr = new_ce();
    final_addr->op.kind = OP_PLUS;
    final_addr->arg1 = addr->result;
    final_addr->arg2 = elem_addr->result;
    final_addr->result = *(new_temp());
    add_code(final_addr);
}